#pragma once
#include <map>
#include "Core/tspch.h"
#include "Mesh.h"
#include "SceneManager/Node.h"
#include "Bone.h"

namespace TS_ENGINE {

	class Model
	{
	public:
		Model();
		Model(const std::string& modelPath);		
		~Model();		
		
		void LoadModel(const std::string& modelPath);
		void CopyFrom(Ref<Model> model);

		Ref<Node> GetRootNode() { return mRootNode; }	
		std::unordered_map<std::string, Ref<Bone>>& GetBoneInfoMap() { return mBoneInfoMap;  };
	private:
		Ref<Texture2D> ProcessTexture(aiMaterial* _assimpMaterial, aiTextureType _textureType, uint32_t _numMaps);	// Process Texture		
		Ref<Material> ProcessMaterial(aiMaterial* _assimpMaterial);													// Process Material		
		Ref<Mesh> ProcessMesh(aiMesh* aiMesh, const aiScene* scene);												// Process Mesh
		Ref<Node> ProcessNode(aiNode* aiNode, Ref<Node> _parentNode, const aiScene* scene);							// Process Node
		
#pragma region Bone related functions
	public:
		Ref<Bone> FindBoneByName(std::string _name);
	private:
		void ExtractBoneWeightForVertices(std::vector<Vertex>& _vertices, aiMesh* _aiMesh, const aiScene* _aiScene);
		void SetVertexBoneDataToDefault(Vertex& _vertex);
		void SetVertexBoneData(Vertex& _vertex, int _boneID, float _weight);
		
		// Sets nodes for bones
		void SetNodesForBones();		
		// Created GUI nodes for bones
		void InitializeBones();
	public:
		void UpdateBone(Ref<Shader> _shader);
		void RenderBones(Ref<Shader> _shader);
#pragma endregion

	private:
		const aiScene* mAssimpScene;
		uint32_t mRendererID;
		AssimpMaterial mAssimpMaterial;		
		std::string mModelDirectory;

		Ref<Node> mRootNode;														// Root Node
		
		std::unordered_map<std::string, Ref<Material>> mProcessedMaterials = {};	// Processed Materials
		std::unordered_map<std::string, Ref<Mesh>> mProcessedMeshes = {};			// Processed Meshes
		std::unordered_map<std::string, Ref<Node>> mProcessedNodes = {};			// Processed Nodes
		std::unordered_map<std::string, Ref<Bone>> mBoneInfoMap;					// Name & Bone Map

		int mBoneCounter = 0;
	};
}

