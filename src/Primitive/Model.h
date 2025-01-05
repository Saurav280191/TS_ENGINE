#pragma once
#include <map>
#include "Core/tspch.h"
#include "Mesh.h"
#include "Primitive/Bone.h"

namespace TS_ENGINE {

	class Bone;
	class Model
	{
	public:
		Model();
		Model(const std::string& modelPath);		
		~Model();		
		
		void LoadModel(const std::string& modelPath);
		void CopyFrom(Ref<Model> model);

		void UpdateBoneTransforms();
		
		Ref<Node> GetRootNode() { return mRootNode; }		
		
		void RenderBones(Ref<Shader> _shader);

		// Gets added while processing meshes
		std::unordered_map<std::string, Ref<Bone>> mBones;						// Bones
		Ref<Line> GetBoneLines() { return mBoneLines; }
	private:
		Ref<Texture2D> ProcessTexture(aiMaterial* _assimpMaterial,
			aiTextureType _textureType, uint32_t _numMaps);						// Process Texture
		
		Ref<Material> ProcessMaterial(aiMaterial* _assimpMaterial);				// Process Material
		
		Ref<Mesh> ProcessMesh(aiMesh* aiMesh, const aiScene* scene);

		// Process Mesh
		Ref<Node> ProcessNode(aiNode* aiNode, 
			Ref<Node> _parentNode, const aiScene* scene);						// Process Node
	
		const aiScene* mAssimpScene;
		uint32_t mRendererID;
		AssimpMaterial mAssimpMaterial;		
		std::string mModelDirectory;
		
		// Contains the point to render bone GUI
		Ref<Line> mBoneLines;

		Ref<Node> mRootNode;													// Root Node
		
		std::unordered_map<std::string, Ref<Material>> mProcessedMaterials = {};// Processed Materials
		std::unordered_map<std::string, Ref<Mesh>> mProcessedMeshes = {};		// Processed Meshes
		std::vector<Ref<Node>> mProcessedNodes = {};							// Processed Nodes
	};
}

