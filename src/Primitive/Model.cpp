#include "tspch.h"
#include "Model.h"
#include "Utils/Utility.h"
#include "Core/Application.h"
#include "Renderer/MaterialManager.h"
#include "Core/Factory.h"

namespace TS_ENGINE {

	Model::Model() :
		mAssimpScene(nullptr),
		mRendererID(0)
	{

	}

	Model::Model(const std::string& modelPath)
	{
		mAssimpScene = nullptr;
		mRendererID = 0;

		// Fetch Model Directory
		Utility::GetDirectory(modelPath, mModelDirectory);
		// Load Model(ProcessMesh, ProcessMaterial, ProcessAnimations etc..)
		LoadModel(modelPath);
	}

	Model::~Model()
	{
		mRendererID = -1;
		mModelDirectory = "";
		mRootNode = nullptr;

		mProcessedMaterials.clear();
		mProcessedMeshes.clear();
		mProcessedNodes.clear();
	}

	void Model::LoadModel(const std::string& modelPath)
	{
		TS_CORE_TRACE("Loading model from: {0}", modelPath);

		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, 0);
		mAssimpScene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);

		if (!mAssimpScene || mAssimpScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !mAssimpScene->mRootNode)
		{
			TS_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return;
		}

		// Process material
		for (unsigned int i = 0; i < mAssimpScene->mNumMaterials; i++)
		{
			aiMaterial* assimpMaterial = mAssimpScene->mMaterials[i];
			Ref<Material> material = ProcessMaterial(assimpMaterial);
			mProcessedMaterials.insert({ material->GetName(), material });
		}

		// Process meshes
		for (unsigned int i = 0; i < mAssimpScene->mNumMeshes; i++)
		{
			aiMesh* assimpMesh = mAssimpScene->mMeshes[i];
			Ref<Mesh> mesh = ProcessMesh(assimpMesh, mAssimpScene);
			mProcessedMeshes.insert({ mesh->GetName(), mesh });
		}

		// Process Nodes
		mRootNode = ProcessNode(mAssimpScene->mRootNode, nullptr, mAssimpScene);

		// SetHasBoneInfluence for nodes
		for (auto& nodePair : mProcessedNodes)
		{
			for (auto& mesh : nodePair.second->GetMeshes())
			{
				if (mesh->HasBoneInfluence())
				{
					Ref<Node>& node = nodePair.second;
					node->SetHasBoneInfluence(true);
				}
			}
		}

		// The bone data has been created in ProcessMesh function.
		// A node needs to be set for each bone.
		SetNodesForBones();

		// Initialize transform for root node and it's children
		mRootNode->Initialize(mRootNode->mName, NodeType::MODELROOTNODE);

		TS_CORE_TRACE("Model is loaded\n");

		// Set modelRootNodeId for all the model children nodes
		for (auto& child : mRootNode->GetChildren())
		{
			child->SetModelRootNodeId(mRootNode->mId);
		}

		// Create bone gui nodes
		CreateBoneGuis();

		// Process animations
		if (mAssimpScene->HasAnimations())
		{
			for (unsigned int i = 0; i < mAssimpScene->mNumAnimations; ++i)
			{
				Ref<Animation> animation = CreateRef<Animation>(mAssimpScene->mAnimations[i]);

				// Add processed animation to root node to show in Inspector panel
				mRootNode->AddAnimation(animation);
			}

			// Set default current animation to first animation
			mRootNode->SetCurrentAnimation(mAssimpScene->mAnimations[0]->mName.C_Str());
		}
		else
		{
			TS_CORE_TRACE("No animations found!");
		}
	}

	/*void Model::ProcessEmbeddedTextures()//Don't use this. This can cause issues
	{
		for (unsigned int i = 0; i < mAssimpScene->mNumTextures; i++)
		{
			aiTexture* aiTex = mAssimpScene->mTextures[i];
			TS_CORE_INFO("Processing embedded aiTex named : {0}", aiTex->mFilename.C_Str());
			Ref<Texture2D> tex2D = Texture2D::Create(aiTex->mFilename.C_Str(), reinterpret_cast<unsigned char*>(aiTex->pcData), aiTex->mWidth);

			mProcessedEmbeddedTextures.insert(std::pair<std::string, Ref<Texture2D>>(aiTex->mFilename.C_Str(), tex2D));
		}
	}*/

	Ref<Node> Model::ProcessNode(aiNode* aiNode, Ref<Node> _parentNode, const aiScene* scene)
	{
		Ref<Node> node = CreateRef<Node>();			
		std::string nodeName = aiNode->mName.C_Str();
		node->mName = nodeName;							// Name

		aiVector3D position;
		aiQuaternion rotation;
		aiVector3D scale;
		aiNode->mTransformation.Decompose(scale, rotation, position);

		node->SetPosition(position);					// Position
		node->SetRotation(rotation);					// Rotation
		node->SetScale(scale);							// Scale

		if (_parentNode)
		{
			_parentNode->AddChild(node);				// ParentNode
		}

		// Process Bones Or Meshes
		if (aiNode->mNumMeshes == 0)// *** Process Bone ***
		{
			node->Initialize(aiNode->mName.C_Str(), NodeType::BONE);	// Register Node As BoneType
		}
		else						// *** Process Meshes ***
		{
			for (GLuint i = 0; i < aiNode->mNumMeshes; i++)
			{
				aiMesh* assimpMesh = scene->mMeshes[aiNode->mMeshes[i]];
				// Fetch mesh from mProcessedMeshes map and add to node's meshes
				Ref<Mesh> processedMesh = mProcessedMeshes[assimpMesh->mName.C_Str()];
				TS_CORE_ASSERT(processedMesh);
				node->AddMesh(processedMesh);
			}

			node->Initialize(aiNode->mName.C_Str(), NodeType::MESH);	// Register Node As MeshType
		}

		// Process nodes
		for (GLuint i = 0; i < aiNode->mNumChildren; i++)
		{
			Ref<Node> childNode = ProcessNode(aiNode->mChildren[i], node, scene);
		}

		mProcessedNodes.insert({ node->mName, node });					// Add Processed Node

		return node;
	}

	Ref<Mesh> Model::ProcessMesh(aiMesh* aiMesh, const aiScene* scene)
	{
		TS_CORE_TRACE("Processing mesh named: {0}", aiMesh->mName.C_Str());

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		std::string meshName = aiMesh->mName.C_Str();

		// Fetch Vertices From AssimpMesh
		for (GLuint i = 0; i < aiMesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.ResetBoneInfoToDefault();

			// Position
			Vector4 position = Vector4(Utility::AssimpVec3ToGlmVec3(aiMesh->mVertices[i]), 1.0f);

			// UV
			Vector2 uv(0.0f);
			if (aiMesh->mTextureCoords[0])
			{
				uv.x = aiMesh->mTextureCoords[0][i].x;
				uv.y = aiMesh->mTextureCoords[0][i].y;
			}

			// Normal
			Vector3 normal(0.0f);
			if (aiMesh->HasNormals())
			{
				normal = Utility::AssimpVec3ToGlmVec3(aiMesh->mNormals[i]);
			}

			vertex.position = position;
			vertex.texCoord = uv;
			vertex.normal = normal;

			vertices.push_back(vertex);			// Add to mesh vertices
		}

		// Fetch Indices From AssimpMesh
		for (GLuint i = 0; i < aiMesh->mNumFaces; i++)
		{
			aiFace face = aiMesh->mFaces[i];

			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// Set BoneId and BoneWeight for vertices
		ExtractBoneWeightForVertices(vertices, aiMesh);
		
		// Fetch Material Info From mProcessedMaterials
		Ref<Material> material = nullptr;

		if (aiMesh->mMaterialIndex >= 0)
		{
			aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];

			// Material name should not be null
			TS_CORE_ASSERT(mProcessedMaterials[aiMat->GetName().C_Str()]);
			material = mProcessedMaterials[aiMat->GetName().C_Str()];
		}

		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetName(meshName);		// Name
		mesh->SetVertices(vertices);	// Vertices
		mesh->SetIndices(indices);		// Indices		
		mesh->SetMaterial(material);	// Materials

		for(auto& vertex : mesh->GetVertices())
		{
			for(auto& weight : vertex.weights)
			{
				if (weight != 0.0f)
				{
					mesh->SetHasBoneInfluence(true);
					break;
				}
			}

			if (mesh->HasBoneInfluence())
			{
				break;
			}
		}

		// Create mesh
		mesh->Create();

		return mesh;
	}

	Ref<Texture2D> Model::ProcessTexture(aiMaterial* _assimpMaterial, aiTextureType _textureType, uint32_t _numMaps)
	{
		for (uint32_t i = 0; i < _numMaps; i++)
		{
			aiString texturePath;

			if (_assimpMaterial->GetTexture(_textureType, i, &texturePath) == aiReturn_SUCCESS)
			{
				Ref<Texture2D> texture = nullptr;

				//if (Ref<Texture2D> embeddedTex = mProcessedEmbeddedTextures[texturePath.C_Str()])
				//{
				//	TS_CORE_INFO("Found diffuse map named {0} from already processed embedded textures", texturePath.C_Str());
				//	mTexture = embeddedTex;
				//}
				//else
				{
					const aiTexture* embeddedTexture = mAssimpScene->GetEmbeddedTexture(texturePath.C_Str());

					if (embeddedTexture)
					{
						//TS_CORE_INFO("Processing embedded aiTex named : {0}", embeddedTexture->mFilename.C_Str());
						Ref<Texture2D> embeddedTex = Texture2D::Create(embeddedTexture->mFilename.C_Str(), reinterpret_cast<unsigned char*>(embeddedTexture->pcData), embeddedTexture->mWidth);
						//mProcessedEmbeddedTextures.insert(std::pair<std::string, Ref<Texture2D>>(aiTex->mFilename.C_Str(), embeddedTex));
						texture = embeddedTex;
					}
					else
					{
						//TS_CORE_INFO("No embedded diffuse map named {0} found", texturePath.C_Str());
						texture = Texture2D::Create(mModelDirectory + "\\" + texturePath.C_Str());
					}
				}

				return texture;
			}
		}

		return nullptr;
	}

	Ref<Material> Model::ProcessMaterial(aiMaterial* _assimpMaterial)
	{
		if (_assimpMaterial->Get(AI_MATKEY_NAME, mAssimpMaterial.name) != AI_SUCCESS)
		{
			TS_CORE_ERROR("Could not fetch material name!");
		}

		//_assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, mMaterial.ambient);
		_assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, mAssimpMaterial.diffuse);
		_assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, mAssimpMaterial.specular);
		_assimpMaterial->Get(AI_MATKEY_OPACITY, mAssimpMaterial.opacity);
		_assimpMaterial->Get(AI_MATKEY_SHININESS, mAssimpMaterial.shininess);

		//TS_CORE_INFO("Material {0} has {1} diffuse textures", this->material.name.C_Str(), diffTexCount);

		//TODO: Add support for other type of textures;
		/*uint32_t ambientTexCount = material->GetTextureCount(aiTextureType_AMBIENT);
		uint32_t heightMapTexCount = material->GetTextureCount(aiTextureType_HEIGHT);
		uint32_t opacityTexCount = material->GetTextureCount(aiTextureType_OPACITY);
		uint32_t displacementTexCount= material->GetTextureCount(aiTextureType_DISPLACEMENT);
		uint32_t shininessTexCount = material->GetTextureCount(aiTextureType_SHININESS);
		uint32_t lightMapTexCount = material->GetTextureCount(aiTextureType_LIGHTMAP);
		uint32_t reflectionTexCount = material->GetTextureCount(aiTextureType_REFLECTION);*/

		std::string materialName = mAssimpMaterial.name.C_Str();

		Vector4 ambientColor(mAssimpMaterial.ambient.r, mAssimpMaterial.ambient.g, mAssimpMaterial.ambient.b, 1);
		Vector4 diffuseColor(mAssimpMaterial.diffuse.r, mAssimpMaterial.diffuse.g, mAssimpMaterial.diffuse.b, 1);
		Vector4 specularColor(mAssimpMaterial.specular.r, mAssimpMaterial.specular.g, mAssimpMaterial.specular.b, 1);
		float shininess = mAssimpMaterial.shininess;

		uint32_t numDiffuseMaps = _assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE);
		uint32_t numSpecularMaps = _assimpMaterial->GetTextureCount(aiTextureType_SPECULAR);
		uint32_t numNormalMaps = _assimpMaterial->GetTextureCount(aiTextureType_NORMALS);
		uint32_t numMetallicMaps = _assimpMaterial->GetTextureCount(aiTextureType_METALNESS);
		uint32_t numEmissiveMaps = _assimpMaterial->GetTextureCount(aiTextureType_EMISSIVE);

		// Process textures
		Ref<Texture2D> diffuseMap = ProcessTexture(_assimpMaterial, aiTextureType_DIFFUSE, numDiffuseMaps);
		Ref<Texture2D> specularMap = ProcessTexture(_assimpMaterial, aiTextureType_SPECULAR, numSpecularMaps);
		Ref<Texture2D> normalMap = ProcessTexture(_assimpMaterial, aiTextureType_NORMALS, numNormalMaps);
		//Ref<Texture2D> metallicMap = ProcessTexture(_assimpMaterial, aiTextureType_METALNESS, numMetallicMaps);
		//Ref<Texture2D> emmisiveMap = ProcessTexture(_assimpMaterial, aiTextureType_EMISSIVE, numEmissiveMaps);

		// Get Unlit Material And Pass To Material
		Ref<Material> unlitMaterial = MaterialManager::GetInstance()->GetMaterial("Unlit");

		Ref<Material> material = nullptr;

		material = CreateRef<Material>(unlitMaterial);

		material->SetName(materialName);			// Name
		material->SetAmbientColor(ambientColor);	// Ambient Color
		material->SetDiffuseColor(diffuseColor);	// Diffuse Color
		material->SetSpecularColor(specularColor);	// Specular Color
		material->SetShininess(shininess);			// Shininess
		if (diffuseMap)
			material->SetDiffuseMap(diffuseMap);	// Diffuse Map
		else if (specularMap)
			material->SetSpecularMap(specularMap);	// Specualr Map
		else if (normalMap)
			material->SetNormalMap(normalMap);		// Normal Map
		//else if(metallicMap)
		//	material->SetMetallicMap(metallicMap);	// Metallic Map
		//else if(emmisiveMap)
		//	material->SetEmmisiveMap(emmisiveMap);	// Emmisive Map

		return material;
	}
	
	const Ref<Node> Model::FindNodeByName(const std::string& _name)
	{
		if (mProcessedNodes[_name])
		{
			return mProcessedNodes[_name];
		}
		else
		{
			TS_CORE_ERROR("Could not find node named: " + _name);
			return nullptr;
		}
	}

	const Ref<Bone>& Model::FindBoneByName(const std::string& _name)
	{
		mBoneInfoMap[_name] ? void() : TS_CORE_ERROR("Node named: {0} not found", _name);
		return mBoneInfoMap[_name];	
	}

	void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& _vertices, aiMesh* _aiMesh)
	{
		for (int boneIndex = 0; boneIndex < (int)_aiMesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;

			std::string boneName = _aiMesh->mBones[boneIndex]->mName.C_Str();

			if (mBoneInfoMap.find(boneName) == mBoneInfoMap.end())
			{
				Ref<Bone> bone = CreateRef<Bone>();
				bone->SetParams(mBoneCounter, Utility::AssimpMat4ToGlmMat4(_aiMesh->mBones[boneIndex]->mOffsetMatrix));
				mBoneInfoMap[boneName] = bone;
				boneID = mBoneCounter;
				mBoneCounter++;
			}
			else
			{
				boneID = mBoneInfoMap[boneName]->GetId();
			}

			TS_CORE_ASSERT(boneID != -1);

			auto weights = _aiMesh->mBones[boneIndex]->mWeights;
			int numWeights = _aiMesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;

				TS_CORE_ASSERT(vertexId <= _vertices.size());

				_vertices[vertexId].SetBoneIdAndWeight(boneID, weight);
			}
		}
	}

	void Model::SetNodesForBones()
	{
		for (auto& [name, bone] : mBoneInfoMap)
		{
			if (bone)
			{
				Ref<Node> node = mProcessedNodes[name];
				bone->SetNode(node);
				node->SetBone(bone);
			}
		}
	}
	
	void Model::CreateBoneGuis()
	{
		for (auto& [name, bone] : mBoneInfoMap)
		{
			if (bone)
			{
				bone->CreateGui();
			}
		}
	}
}