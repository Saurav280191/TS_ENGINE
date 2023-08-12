#include "tspch.h"
#include "Model.h"

namespace TS_ENGINE {

	Model::Model()
	{
		mPrimitiveType = PrimitiveType::MODEL;
	}

	Model::Model(const std::string& modelPath)
	{
		mPrimitiveType = PrimitiveType::MODEL;
		this->LoadModel(modelPath);
	}

	void Model::CopyFrom(Ref<Model> model)
	{
		this->AddMeshes(model->GetMeshes());
		this->mTexture = model->mTexture;
	}


	Model::~Model()
	{

	}

	void Model::LoadModel(const std::string& modelPath)
	{
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate);

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			TS_CORE_ERROR("ERROR::ASSIMP:: {0}", importer.GetErrorString());
			return;
		}

		this->ProcessNode(scene->mRootNode, scene);

		Initialize();
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{		
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->AddMesh(ProcessMesh(mesh, scene));
		}
		
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene);
		}
	}

	Ref<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			
			Vector4 vector; 

			// Position
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vector.w = 1;
			vertex.position = vector;

			//Color
			vertex.color = GetColor();

			// Normal
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			//vertex.normal = vector;
			

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;				
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoord = vec;
			}
			else
			{
				vertex.texCoord = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}
		
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			material->Get(AI_MATKEY_NAME, this->material.name);
			material->Get(AI_MATKEY_COLOR_AMBIENT, this->material.ambient);
			material->Get(AI_MATKEY_COLOR_DIFFUSE, this->material.diffuse);
			material->Get(AI_MATKEY_COLOR_SPECULAR, this->material.specular);
			material->Get(AI_MATKEY_OPACITY, this->material.opacity);
			material->Get(AI_MATKEY_SHININESS, this->material.shininess);

			uint32_t diffTexCount = material->GetTextureCount(aiTextureType_DIFFUSE);
			//TS_CORE_INFO("Material {0} has {1} diffuse textures", this->material.name.C_Str(), diffTexCount);
			
			//TODO: Add support for other type of textures;
			/*uint32_t specTexCount = material->GetTextureCount(aiTextureType_SPECULAR);
			uint32_t ambientTexCount = material->GetTextureCount(aiTextureType_AMBIENT);
			uint32_t emmisiveTexCount = material->GetTextureCount(aiTextureType_EMISSIVE);
			uint32_t heightMapTexCount = material->GetTextureCount(aiTextureType_HEIGHT);
			uint32_t normTexCount = material->GetTextureCount(aiTextureType_NORMALS);
			uint32_t opacityTexCount = material->GetTextureCount(aiTextureType_OPACITY);
			uint32_t displacementTexCount= material->GetTextureCount(aiTextureType_DISPLACEMENT);
			uint32_t shininessTexCount = material->GetTextureCount(aiTextureType_SHININESS);
			uint32_t metalTexCount = material->GetTextureCount(aiTextureType_METALNESS);
			uint32_t lightMapTexCount = material->GetTextureCount(aiTextureType_LIGHTMAP);
			uint32_t reflectionTexCount = material->GetTextureCount(aiTextureType_REFLECTION);*/

			for (uint32_t i = 0; i < diffTexCount; i++)
			{
				aiString texturefile;

				if (material->GetTexture(aiTextureType_DIFFUSE, i, &texturefile) == aiReturn_SUCCESS)
				{

					aiTexture* aiTex = scene->mTextures[i];
					mTexture = Texture2D::Create(texturefile.C_Str(), reinterpret_cast<unsigned char*>(aiTex->pcData), aiTex->mWidth);
				}
				
			}
		}

		Ref<Mesh> mMesh = CreateRef<Mesh>();
		mMesh->SetVertices(vertices);
		mMesh->SetIndices(indices);
		mMesh->Create();

		return mMesh;
	}
}