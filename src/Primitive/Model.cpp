#include "tspch.h"
#include "Model.h"
#include "Utils/Utility.h"
#include "Core/Application.h"
#include "Renderer/MaterialManager.h"
#include "Core/Factory.h"

//#define AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS

namespace TS_ENGINE {

	Model::Model()
	{
		mAssimpScene = nullptr;
		mRendererID = 0;
	}

	Model::Model(const std::string& modelPath)
	{
		mAssimpScene = nullptr;
		mRendererID = 0;

		Utility::GetDirectory(modelPath, mModelDirectory);
		this->LoadModel(modelPath);
	}

	void Model::CopyFrom(Ref<Model> model)// TODO:: Check why this does not work
	{
		mAssimpScene = model->mAssimpScene;
		mRendererID = model->mRendererID;
		mAssimpMaterial = model->mAssimpMaterial;
		mModelDirectory = model->mModelDirectory;
		mRootNode = model->mRootNode;
		mProcessedMaterials = model->mProcessedMaterials;
		mProcessedMeshes = model->mProcessedMeshes;
		mProcessedNodes = model->mProcessedNodes;
	}

	void Model::UpdateBoneTransforms()
	{
		for (auto& bone : mBones)
		{
			bone.second->UpdateBoneGuiTransforms();
		}
	}

	void Model::RenderBones(Ref<Shader> _shader)
	{
		for (auto& bone : mBones)
		{
			bone.second->Render(_shader);
		}
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
		
		// Initialize transform for root node and it's children
		mRootNode->Initialize(mRootNode->mName, EntityType::MODELROOTNODE);

		for (auto& [boneName, bone] : mBones)
		{
			bone->InitializeBones();
		}

		// TODO: Process animations
		if (mAssimpScene->HasAnimations())
		{
			for (unsigned int i = 0; i < mAssimpScene->mNumAnimations; ++i)
			{
				aiAnimation* animation = mAssimpScene->mAnimations[i];
				TS_CORE_TRACE("Animation " + std::to_string(i) + ": " + animation->mName.C_Str());
				TS_CORE_TRACE("Duration: " + std::to_string(animation->mDuration));
				TS_CORE_TRACE("Ticks per second: " + std::to_string(animation->mTicksPerSecond));
			}
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
		node->SetNodeRef(node);							// NodeRef		
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
			if (mBones.find(nodeName) == mBones.end())
			{
				TS_CORE_ERROR("Unable to find bone named: {0}", nodeName);
			}
			else
			{	
				mBones[nodeName]->SetNode(node);		// Bones
			}

			node->Initialize(aiNode->mName.C_Str(), EntityType::BONE);	// Register Entity As Bone
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

			node->Initialize(aiNode->mName.C_Str(), EntityType::MESH);	// Register Entity As Mesh
		}

		// Process nodes
		for (GLuint i = 0; i < aiNode->mNumChildren; i++)
		{
			Ref<Node> childNode = ProcessNode(aiNode->mChildren[i], node, scene);
		}

		mProcessedNodes.push_back(node);								// Add Processed Node

		return node;
	}

	Ref<Mesh> Model::ProcessMesh(aiMesh* aiMesh, const aiScene* scene)
	{	
		TS_CORE_TRACE("Processing mesh named: {0}", aiMesh->mName.C_Str());

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		
		// Fetch Vertices From AssimpMesh
		for (GLuint i = 0; i < aiMesh->mNumVertices; i++)
		{
			Vertex vertex;
			
			Vector4 vector; 

			// Position
			vector.x = aiMesh->mVertices[i].x;
			vector.y = aiMesh->mVertices[i].y;
			vector.z = aiMesh->mVertices[i].z;
			vector.w = 1;
			vertex.position = vector;
			
			// Normal
			if (aiMesh->HasNormals())
			{
				vector.x = aiMesh->mNormals[i].x;
				vector.y = aiMesh->mNormals[i].y;
				vector.z = aiMesh->mNormals[i].z;
				//vertex.normal = vector;
			}

			if (aiMesh->mTextureCoords[0])
			{
				glm::vec2 vec;				
				vec.x = aiMesh->mTextureCoords[0][i].x;
				vec.y = aiMesh->mTextureCoords[0][i].y;
				vertex.texCoord = vec;
			}
			else
			{
				vertex.texCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
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

		// Fetch Bone Info From AssimpMesh
		TS_CORE_INFO("Number of bones in " + std::string(aiMesh->mName.C_Str()) + ": " + std::to_string(aiMesh->mNumBones));

		if (aiMesh->HasBones())
		{
			for (unsigned int boneId = 0; boneId < aiMesh->mNumBones; boneId++)
			{
				aiBone* assimpBone = aiMesh->mBones[boneId];
				std::string boneName = assimpBone->mName.C_Str();

				// If bone is not already created and added to map
				if (mBones.find(boneName) == mBones.end())
				{
					Ref<Bone> bone = CreateRef<Bone>();

					// Store the offset matrix
					Matrix4 offsetMatrix = Matrix4(
						assimpBone->mOffsetMatrix.a1, assimpBone->mOffsetMatrix.a2, assimpBone->mOffsetMatrix.a3, assimpBone->mOffsetMatrix.a4,
						assimpBone->mOffsetMatrix.b1, assimpBone->mOffsetMatrix.b2, assimpBone->mOffsetMatrix.b3, assimpBone->mOffsetMatrix.b4,
						assimpBone->mOffsetMatrix.c1, assimpBone->mOffsetMatrix.c2, assimpBone->mOffsetMatrix.c3, assimpBone->mOffsetMatrix.c4,
						assimpBone->mOffsetMatrix.d1, assimpBone->mOffsetMatrix.d2, assimpBone->mOffsetMatrix.d3, assimpBone->mOffsetMatrix.d4
					);

					// Assign weights to vertices				
					std::vector<VertexWeight> vertexWeights = {};

					for (unsigned int weightIndex = 0; weightIndex < assimpBone->mNumWeights; weightIndex++)
					{
						aiVertexWeight weight = assimpBone->mWeights[weightIndex];
						vertices[weight.mVertexId].AddBoneData(weight);
						
						VertexWeight vertexWeight(weight);
						vertexWeights.push_back(vertexWeight);
					}

					// Set bone params and store in mBones
					bone->SetParams(boneName, boneId, vertexWeights, offsetMatrix);
					mBones[boneName] = bone;
				}
			}
		}

		Ref<Material> material = nullptr;

		// Fetch Material Info From AssimpMesh
		if (aiMesh->mMaterialIndex >= 0)
		{
			aiMaterial* aiMat = scene->mMaterials[aiMesh->mMaterialIndex];

			// Material name should not be null
			TS_CORE_ASSERT(mProcessedMaterials[aiMat->GetName().C_Str()]);
			material = mProcessedMaterials[aiMat->GetName().C_Str()];
		}

		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetName(aiMesh->mName.C_Str());	// Name
		mesh->SetVertices(vertices);			// Vertices
		mesh->SetIndices(indices);				// Indices		
		mesh->SetMaterial(material);			// Materials

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
		std::string materialName = _assimpMaterial->GetName().C_Str();

		_assimpMaterial->Get(AI_MATKEY_NAME, this->mAssimpMaterial.name);
		//aiMat->Get(AI_MATKEY_COLOR_AMBIENT, this->mMaterial.ambient);
		_assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, this->mAssimpMaterial.diffuse);
		_assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, this->mAssimpMaterial.specular);
		_assimpMaterial->Get(AI_MATKEY_OPACITY, this->mAssimpMaterial.opacity);
		_assimpMaterial->Get(AI_MATKEY_SHININESS, this->mAssimpMaterial.shininess);

		//TS_CORE_INFO("Material {0} has {1} diffuse textures", this->material.name.C_Str(), diffTexCount);

		//TODO: Add support for other type of textures;
		/*uint32_t ambientTexCount = material->GetTextureCount(aiTextureType_AMBIENT);		
		uint32_t heightMapTexCount = material->GetTextureCount(aiTextureType_HEIGHT);
		uint32_t opacityTexCount = material->GetTextureCount(aiTextureType_OPACITY);
		uint32_t displacementTexCount= material->GetTextureCount(aiTextureType_DISPLACEMENT);
		uint32_t shininessTexCount = material->GetTextureCount(aiTextureType_SHININESS);		
		uint32_t lightMapTexCount = material->GetTextureCount(aiTextureType_LIGHTMAP);
		uint32_t reflectionTexCount = material->GetTextureCount(aiTextureType_REFLECTION);*/

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
		Ref<Material> unlitMaterial = MaterialManager::GetInstance()->GetUnlitMaterial();
		Ref<Material> skinnedMeshUnlitMaterial = MaterialManager::GetInstance()->GetSkinnedMeshUnlitMaterial();
		
		Ref<Material> material = nullptr;
		
		if(mBones.size() > 0)
			material = CreateRef<Material>(skinnedMeshUnlitMaterial);
		else
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
}