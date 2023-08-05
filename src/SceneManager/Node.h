#pragma once
#include "tspch.h"
#include <memory>
#include "Core/Transform.h"
#include "Renderer/Shader.h"
#include "Core/Object.h"
//#include "Core/GameObject.h"
//#include "Renderer/Camera.h"

namespace TS_ENGINE
{
	class Object;
	class Transform;
	class Node
	{
	public:
		Node();
		Node(Node* node);
		void Destroy();

		void SetName(std::string name);
		void AttachObject(Ref<Object> object);
		//void AttachGameObject(Ref<GameObject> gameObject);
		//void AttachCamera(Ref<Camera> camera);
		void AddChild(Ref<Node> child);
		void RemoveChild(Ref<Node> child);
		void RemoveAllChildren();

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

		const Ref<Object> GetAttachedObject() const
		{
			if (mAttachedObject)
				return mAttachedObject;
			else
			{
				TS_CORE_ERROR("There is no gameobject attached to node: {0}", mName);
				return nullptr;
			}
		}
		
		/*const Ref<GameObject> GetAttachedGameObject() const
		{
			if(mAttachedGameObject)
				return mAttachedGameObject;
			else
				TS_CORE_ERROR("There is no gameobject attached to node: {0}", mName);
		}*/
		
		/*const Ref<Camera> GetAttachedCamera() const
		{
			if (mAttachedCamera)
				return mAttachedCamera;
			else
				TS_CORE_ERROR("There is no camera attached to node: {0}", mName);
		}*/

		bool HasAttachedObject()
		{
			if (mAttachedObject)// || mAttachedGameObject)// || mAttachedCamera)
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

		Ref<Node> GetParentNode()
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
		Ref<Node> mParentNode;
		std::vector<Ref<Node>> mChildren;

		Ref<Transform> mTransform;
		Ref<Object> mAttachedObject;
		//Ref<GameObject> mAttachedGameObject;
		//Ref<Camera> mAttachedCamera;

#ifdef TS_ENGINE_EDITOR
		bool mIsVisibleInEditor;
#endif
	};
}

