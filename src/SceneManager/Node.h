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

		void SetName(std::string name);

		void AttachObject(Ref<Object> object);
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

		void AddChild(Ref<Node> child);
		void RemoveChild(Ref<Node> child);
		void RemoveAllChildren();

		void SetEntityType(EntityType entityType);
		EntityType GetEntityType()
		{
			return mEntityType;
		}
		void InitializeTransformMatrices();
		
		void SetPosition(float* pos);
		void SetEulerAngles(float* eulerAngles);
		void SetScale(float* scale);

		void UpdateTransformationMatrices(Matrix4 transformationMatrix);
		void Update(Ref<Shader> shader, float deltaTime);
		//Getters
		const std::string& GetName() const
		{
			return mName;
		}
		const Ref<Node> GetChildAt(uint32_t childIndex) const;
		const std::vector<Ref<Node>> GetChildren() const
		{
			return mChildren;
		}
		const Ref<Transform> GetTransform() const
		{
			return mTransform;
		}
		
		void LookAt(Ref<Node> targetNode);

		bool HasAttachedObject()
		{
			if (mAttachedObject)
				return true;
			else
				return false;
		}
		size_t GetChildCount()
		{
			return mChildren.size();
		}

		//For IMGUI
		bool m_Enabled = false;

		Node* GetParentNode()
		{
			return mParentNode;
		}

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
		std::string mName;		
		Node* mParentNode;//This is not a smart pointer. Handler the cleaning manually.
		std::vector<Ref<Node>> mChildren;

		Ref<Transform> mTransform;
		Ref<Object> mAttachedObject;		

#ifdef TS_ENGINE_EDITOR
		bool mIsVisibleInEditor;
#endif

		EntityType mEntityType;
	};
}

