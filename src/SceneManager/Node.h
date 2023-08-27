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
	/*enum class PrimitiveType
	{
		QUAD,
		CUBE,
		SPHERE,
		CYLINDER,
		CONE,
		EMPTY
	};*/

	class Transform;
	class Node
	{
	public:
		Node();
		~Node();

		void Destroy();

		//void SetEntityType(EntityType entityType); 
		void SetName(const std::string& name);

		//This is important to keep a reference of the smart pointer create in Object class
		void SetNodeRef(Ref<Node> node);
		void SetParent(Ref<Node> parentNode);		
		void SetPosition(float* pos);
		void SetPosition(const Vector3& pos);
		void SetEulerAngles(float* eulerAngles);
		void SetEulerAngles(const Vector3& eulerAngles);
		void SetScale(float* scale);

		void SetScale(const Vector3& scale);

		void AddChild(Ref<Node> child);
		void RemoveChild(Ref<Node> child);
		void RemoveAllChildren();
		void UpdateSiblings();

		/// <summary>
		/// Updates Model matrix for it's self and for children
		/// </summary>
		void InitializeTransformMatrices();

		void UpdateTransformationMatrices(Matrix4 transformationMatrix);
		void Update(Ref<Shader> shader, float deltaTime);

		void LookAt(Ref<Node> targetNode);

		void AddMesh(Ref<Mesh> mesh);
		void AddMeshes(std::vector<Ref<Mesh>> _meshes);

		void PrintChildrenName();//Only for testing

#ifdef TS_ENGINE_EDITOR
		const bool IsVisibleInEditor() const { return mIsVisibleInEditor; }
		void HideInEditor();
#endif

#pragma region Getters
		Ref<Node> GetNode() const { return mNodeRef; }		
		const Ref<Entity> GetEntity() const { return mEntity; }
		//const EntityType GetEntityType() const { return mEntityType; }
		const std::string& GetName() const { return mName; }
		Ref<Node> GetChildAt(uint32_t childIndex) const;
		Ref<Node> GetParentNode() const { return mParentNode; }
		std::vector<Ref<Node>> GetChildren() const { return mChildren; }
		const std::vector<Ref<Node>> GetSiblings() const { return mSiblings; }
		const Ref<Transform> GetTransform() const { return mTransform; }
		const size_t GetChildCount() const { return mChildren.size(); }
		const std::vector<Ref<Mesh>> GetMeshes() const { return mMeshes; }
		//const PrimitiveType GetPrimitiveType() const { return mPrimitiveType; }
#pragma endregion

	public:
		bool m_Enabled = false;//For IMGUI
	private:
		Ref<Node> mNodeRef;// This will be used for referencing everywhere instead of Node*
		Ref<Entity> mEntity;// Entity
		//EntityType mEntityType;
		std::string mName;
		Ref<Node> mParentNode;
		std::vector<Ref<Node>> mChildren;
		Ref<Transform> mTransform;

		std::vector<Ref<Node>> mSiblings = {};
		std::vector<Ref<Mesh>> mMeshes;
		
		bool mIsVisibleInEditor = true;
	protected:
		//PrimitiveType mPrimitiveType;//Only for GameObject EntityType
	};
}

