#pragma once
#include <map>
#include "Core/tspch.h"
#include "Mesh.h"
#include "SceneManager/Node.h"
#include "Bone.h"
#include "Components/Animation.h"

namespace TS_ENGINE {

	class Model
	{
	public:
		Model();
		Model(const std::string& modelPath);		
		~Model();		
		
		void LoadModel(const std::string& modelPath);		

		Ref<Node> GetRootNode() { return mRootNode; }
	private:
		Ref<Texture2D> ProcessTexture(aiMaterial* _assimpMaterial, aiTextureType _textureType, uint32_t _numMaps);	// Process Texture		
		Ref<Material> ProcessMaterial(aiMaterial* _assimpMaterial);													// Process Material		
		Ref<Mesh> ProcessMesh(aiMesh* aiMesh, const aiScene* scene);												// Process Mesh
		Ref<Node> ProcessNode(aiNode* aiNode, Ref<Node> _parentNode, const aiScene* scene);							// Process Node
		
#pragma region Bone related functions
	public:
		// Used to find node by name in Factory's FindNodeByName function  
		const Ref<Node> FindNodeByName(const std::string& _name);
		// Used to find bone id in Factory's GetBoneIdByName function  
		const Ref<Bone>& FindBoneByName(const std::string& _name);
		
		const std::unordered_map<std::string, Ref<Bone>>& GetBoneInfoMap() { return mBoneInfoMap; }
	private:
		void ExtractBoneWeightForVertices(std::vector<Vertex>& _vertices, aiMesh* _aiMesh);
		
		// Sets nodes for bones
		// Created GUI nodes for bones
		void CreateBoneGuis();
		void SetNodesForBones();
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

