#pragma once
#include "Core/tspch.h"
#include "Mesh.h"
#include <stb_image.h>
#include "SceneManager/Node.h"

namespace TS_ENGINE {

	class Model
	{
	public:
		Model();
		Model(const std::string& modelPath);		
		~Model();		
		
		void LoadModel(const std::string& modelPath);
		void ProcessEmbeddedTextures();
		void CopyFrom(Ref<Model> model);
		
		Ref<Node> GetRootNode() { return mRootNode; }
	private:
		Ref<Node> ProcessNode(aiNode* aiNode, const aiScene* scene);
		void AddMaterialToDictionary(Ref<Material> material);
		Ref<Mesh> ProcessMesh(aiMesh* aiMesh, const aiScene* scene);
		void ProcessMaterial(aiMaterial* aiMat);
	private:
		const aiScene* mAssimpScene;
		uint32_t mRendererID;
		AssimpMaterial mMaterial;
		Ref<Material> mTsMaterial;
		std::string mModelDirectory;
		Ref<Node> mRootNode;
		Ref<Shader> mDefaultShader;
		std::vector<Ref<Node>> mProcessedNodes = {};
		std::vector<Ref<Mesh>> mProcessedMeshes = {};
		std::unordered_map<std::string, Ref<Material>> mProcessedMaterials = {};
		std::unordered_map<std::string, Ref<Texture2D>> mProcessedEmbeddedTextures = {};
		Ref<Texture2D> mTexture;
	};
}

