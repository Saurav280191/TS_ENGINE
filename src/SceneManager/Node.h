#pragma once
#include "tspch.h"

#include "Core/Transform.h"
#include "Core/GameObject.h"

namespace TS_ENGINE
{
	class Node
	{
	public:
		Node();
		void Destroy();

		void SetName(std::string name);
		void AttachGameObject(Ref<GameObject> gameObject);
		void SetParentNode(Ref<Node> parentNode);
		void AddChild(Ref<Node> child);
		void RemoveChild(Ref<Node> child);
		void RemoveAllChildren();

		void Draw(const Matrix4& parentTransformModelMatrix, Ref<Shader> shader);

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
		const Ref<GameObject> GetAttachedGameObject() const
		{
			if (mAttachedGameObject)
				return mAttachedGameObject;
			else
				TS_CORE_ERROR("There is not gameobject attached to this node");
		}
		bool HasAttachedGameObject()
		{
			if (mAttachedGameObject)
				return true;
			else
				return false;
		}
		size_t GetChildCount()
		{
			return mChildren.size();
		}
		void UpdateModelMatrices();
	public:
		//For IMGUI
		bool m_Enabled = false;
	private:
		std::string mName;
		Ref<Node> mParentNode;
		std::vector<Ref<Node>> mChildren;

		Ref<Transform> mTransform;
		Ref<GameObject> mAttachedGameObject;
	};
}

