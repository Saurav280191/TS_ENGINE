#pragma once
#include "tspch.h"
#include <memory>
#include "Core/Transform.h"
#include "Renderer/Shader.h"
#include "Primitive/Mesh.h"
#include "EntityManager/Entity.h"
#include "EntityManager/EntityManager.h"

namespace TS_ENGINE
{
	class Transform;
	class SceneCamera;
	class Node
	{		
	public:
		Node();
		~Node();	

		void Destroy();
		Ref<Node> Duplicate();

		//This is important to keep a reference of the smart pointer create in Object class
		void SetNodeRef(Ref<Node> node);
		void SetParent(Ref<Node> parentNode);
		void ChangeParent(Ref<Node> parentNode);

		void SetPosition(float* pos);
		void SetPosition(float x, float y, float z);
		void SetPosition(const Vector3& pos);
		void SetEulerAngles(float* eulerAngles);
		void SetEulerAngles(float x, float y, float z);
		void SetEulerAngles(const Vector3& eulerAngles);
		void SetScale(float* scale);
		void SetScale(float x, float y, float z);
		void SetScale(const Vector3& scale);
		void SetSceneCamera(Ref<SceneCamera> sceneCamera);

		void AddChild(Ref<Node> child);
		void RemoveChild(Ref<Node> child);
		void RemoveAllChildren();
		void UpdateSiblings();
		void SetSiblingIndex(int index);

		
		// Updates local and global model matrices for itself and for children
		void InitializeTransformMatrices();	
		void UpdateTransformationMatrices(Matrix4 transformationMatrix);
		
		// Registers entity amd initializes transformation matrix
		void Initialize(const std::string& name, const EntityType& entityType);
		
		// Initializes transformation matrices of node and it's children 
		void ReInitializeTransforms();

		// Sets model matrix in shader. Renders mesh. Then updates children.
		void Update(Ref<Shader> shader, float deltaTime);

		void LookAt(Ref<Node> targetNode);

		void ReplaceMesh(Ref<Mesh> mesh);
		void ChangeMesh(PrimitiveType primitiveType);

		void AddMesh(Ref<Mesh> mesh);
		void AddMeshes(std::vector<Ref<Mesh>> meshes);
		void RemoveAllMeshes();
		bool HasMeshes();

		void SetModelPath(std::string modelPath);

		void PrintChildrenName();//Only for testing

		void CloneMeshes(std::vector<Ref<Mesh>> meshes);

#ifdef TS_ENGINE_EDITOR
		const bool IsVisibleInEditor() const { return mIsVisibleInEditor; }
		void HideInEditor();					
#endif

#pragma region Getters
		Ref<Node> GetNode() const { return mNodeRef; }		
		const Ref<Entity> GetEntity() const { return mEntity; }		
		Ref<Node> GetChildAt(uint32_t childIndex) const;
		Ref<Node> GetParentNode() const { return mParentNode; }
		std::vector<Ref<Node>> GetChildren() const { return mChildren; }
		const std::vector<Ref<Node>> GetSiblings() const { return mSiblings; }
		const Ref<Transform> GetTransform() const { return mTransform; }
		const size_t GetChildCount() const { return mChildren.size(); }
		std::vector<Ref<Mesh>> GetMeshes() const { return mMeshes; }
		Ref<SceneCamera> GetSceneCamera() { return mSceneCamera; }
		std::string GetModelPath() { return mModelPath; }
		const int GetSiblingIndex(Ref<Node> node);
#pragma endregion

	public:
#ifdef TS_ENGINE_EDITOR
		bool m_Enabled = true;//For IMGUI
#endif
	private:
		bool mIsInitialized;		
		Ref<Node> mNodeRef;// This will be used for referencing everywhere instead of Node*
		Ref<Entity> mEntity;// Entity		
		Ref<Node> mParentNode;
		std::vector<Ref<Node>> mChildren;
		Ref<Transform> mTransform;

		std::vector<Ref<Node>> mSiblings = {};
		std::vector<Ref<Mesh>> mMeshes;
		std::string mModelPath;
		Ref<SceneCamera> mSceneCamera;// Only used incase of scene camera node
#ifdef TS_ENGINE_EDITOR
		bool mIsVisibleInEditor = true;
#endif
	};
}

