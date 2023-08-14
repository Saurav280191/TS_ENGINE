#pragma once
#include "tspch.h"
#include <memory>
#include "Core/Transform.h"
#include "Renderer/Shader.h"
#include "Core/Object.h"

namespace TS_ENGINE
{
	class Object;
	class Transform;
	class Node
	{
	public:
		Node();
		~Node();

		void Destroy();

		void SetEntityType(EntityType entityType);
		void SetName(std::string name);

		void AttachObject(Ref<Object> object);

		//This is inportant to keep a reference of the smart pointer create in Object class
		void SetNodeRef(Ref<Node> node);
		void SetParent(Ref<Node> parentNode);
		void SetParent(Node* parentNode);
		void AddChild(Ref<Node> child);
		void RemoveChild(Ref<Node> child);
		void RemoveAllChildren();
		void UpdateSiblings();

		void InitializeTransformMatrices();
		
		void SetPosition(float* pos);
		void SetEulerAngles(float* eulerAngles);
		void SetScale(float* scale);

		void UpdateTransformationMatrices(Matrix4 transformationMatrix);
		void Update(Ref<Shader> shader, float deltaTime);
		
		void LookAt(Ref<Node> targetNode);

#pragma region Getters
		Ref<Node> GetNodeRef() const
		{
			return mNodeRef;
		}
		const EntityType GetEntityType() const
		{
			return mEntityType;
		}
		const std::string& GetName() const
		{
			return mName;
		}
		const Ref<Node> GetChildAt(uint32_t childIndex) const;
		Node* GetParentNode() const
		{
			return mParentNode;
		}
		const std::vector<Ref<Node>> GetChildren() const
		{
			return mChildren;
		}		
		const std::vector<Ref<Node>> GetSiblings() const
		{
			return mSiblings;
		}
		const Ref<Transform> GetTransform() const
		{
			return mTransform;
		}
		const bool HasAttachedObject() const
		{
			if (mAttachedObject)
				return true;
			else
				return false;
		}
		const Ref<Object> GetAttachedObject() const
		{
			if (mAttachedObject)
				return mAttachedObject;
			else
			{
				TS_CORE_ERROR("There is no object attached to node: {0}", mName);
				return nullptr;
			}
		}
		const size_t GetChildCount() const
		{
			return mChildren.size();
		}
#pragma endregion

		//For IMGUI
		bool m_Enabled = false;
#ifdef TS_ENGINE_EDITOR
		const bool IsVisibleInEditor()
		{
			return mIsVisibleInEditor;
		}
		void HideInEditor()
		{
			mIsVisibleInEditor = false;
		}
#endif

	private:
		Ref<Node> mNodeRef;
		EntityType mEntityType;
		std::string mName;		
		Node* mParentNode;//This is not a smart pointer. Handle the cleaning manually.
		std::vector<Ref<Node>> mChildren;
		Ref<Transform> mTransform;
		Ref<Object> mAttachedObject;		
		std::vector<Ref<Node>> mSiblings = {};

#ifdef TS_ENGINE_EDITOR
		bool mIsVisibleInEditor;
#endif
	};
}

