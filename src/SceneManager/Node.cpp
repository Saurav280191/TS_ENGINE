#include "tspch.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	Node::Node()
	{
		mTransform = CreateRef<Transform>();
	}

	void Node::Destroy()
	{
		mName = "";
		mTransform.reset();

		if(mAttachedGameObject)
			mAttachedGameObject.reset();

		for (auto& node : mChildren)
			node.reset();

		mParentNode.reset();
		mChildren.clear();
	}

	void Node::SetName(std::string name)
	{
		mName = name;
	}

	void Node::AttachGameObject(Ref<GameObject> gameObject)
	{
		mAttachedGameObject = gameObject;
	}

	void Node::SetParentNode(Ref<Node> parentNode)
	{
		mParentNode = parentNode;
	}

	void Node::AddChild(Ref<Node> child)
	{
		mChildren.push_back(child);
	}

	void Node::RemoveChild(Ref<Node> child)
	{
		auto iter = std::find(mChildren.begin(), mChildren.end(), child);
		
		if (iter != mChildren.end()) 
		{ 
			mChildren.erase(iter);
			TS_CORE_INFO("Element " + child->mName + " removed from vector");
		}
		else 
		{
			TS_CORE_ERROR("Element " + child->mName + " not found in vector");
		}
	}

	void Node::RemoveAllChildren()
	{
		for (auto& child : mChildren)
			child.reset();

		mChildren.clear();
	}

	//If there is no parent set parentTransformModelMatrix to identity
	void Node::Draw(const Matrix4& parentTransformModelMatrix, Ref<Shader> shader)
	{
		mTransform->ComputeModelMatrix(parentTransformModelMatrix);

		shader->SetMat4("u_Model",
			glm::translate(glm::mat4(1), mTransform->GetLocalPosition())
			* glm::rotate(glm::mat4(1), glm::radians( mTransform->GetLocalEulerAngles().x), glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1), glm::radians(-mTransform->GetLocalEulerAngles().y), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1), glm::radians( mTransform->GetLocalEulerAngles().z), glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1), mTransform->GetLocalScale()));

		if (mAttachedGameObject)
			mAttachedGameObject->Draw(shader);

		for(auto& child : mChildren)
		{
			child->Draw(this->mTransform->GetModelMatrix(), shader);
		}
	}
	
	const Ref<Node> Node::GetChildAt(uint32_t childIndex) const
	{
		{
			try
			{
				Ref<Node> child = mChildren[childIndex];
				return child;
			}
			catch (std::out_of_range& e)
			{
				TS_CORE_ERROR(e.what());
			}
		}
	}
	
	/// <summary>
		/// Updates Model matrix for it's self and for children
		/// </summary>
	void Node::UpdateModelMatrices()
	{
		if(mParentNode == nullptr)
			mTransform->ComputeModelMatrix();

		for (auto& child : mChildren)
		{
			child->GetTransform()->ComputeModelMatrix(mTransform->GetModelMatrix());
			child->UpdateModelMatrices();
		}
	}
}