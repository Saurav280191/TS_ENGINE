#include "tspch.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	Node::Node(const std::string& name) :
		mParentNode(nullptr),
		mTransform(CreateRef<Transform>()),
#ifdef TS_ENGINE_EDITOR
		mIsVisibleInEditor(true)
#endif
	{
		mMeshes = {};
		mName = name;
		mEntity = EntityManager::GetInstance()->Initialize(name);// , EntityType::NODE);
	}

	Node::~Node()
	{
		Destroy();//TODO:: Why is it getting destructured
	}

	void Node::Destroy()
	{
		for (auto& mesh : mMeshes)
			mesh->Destroy();

		mMeshes.clear();

		mName = "";
		mTransform.reset();

		//for (auto& node : mChildren)
		//{
		//	//node.reset();
		//	delete &node;
		//}

		mParentNode = nullptr;
		delete mParentNode;

		mChildren.clear();
	}

	/*void Node::SetNodeRef(Ref<Node> node) 
	{ 
		mNodeRef = node; 
	}*/

	/*void Node::SetParent(Ref<Node> parentNode)
	{
		SetParent(parentNode.get());
	}*/

	/*void Node::SetParent(Node* parentNode)
	{
		TS_CORE_INFO("Setting parent of {0} as {1}", mNodeRef->GetName(), parentNode->GetName());

		if (mNodeRef->mParentNode)
		{
			mNodeRef->mParentNode->RemoveChild(mNodeRef);
		}

		parentNode->AddChild(mNodeRef);
		mNodeRef->GetTransform()->ComputeTransformationMatrix(mNodeRef.get(), parentNode);
	}*/

	/*void Node::SetEntityType(EntityType entityType) 
	{ 
		mEntityType = entityType;
	}*/

	void Node::SetName(const std::string& name) 
	{ 
		mName = name; 
	}

	void Node::SetParent(Node* parentNode)
	{
		TS_CORE_INFO("Setting parent of {0} as {1}", mName.c_str(), parentNode->GetName());

		if (this->mParentNode)
		{
			this->mParentNode->RemoveChild(this);
		}

		parentNode->AddChild(this);
		this->GetTransform()->ComputeTransformationMatrix(parentNode);
	}

	void Node::SetPosition(float* pos)
	{
		mTransform->SetLocalPosition(pos);

		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
			child->InitializeTransformMatrices();
	}

	void Node::SetPosition(const Vector3& pos)
	{
		mTransform->SetLocalPosition(pos);

		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
			child->InitializeTransformMatrices();
	}

	void Node::SetEulerAngles(float* eulerAngles)
	{
		mTransform->SetLocalEulerAngles(eulerAngles);

		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
			child->InitializeTransformMatrices();
	}

	void Node::SetEulerAngles(const Vector3& eulerAngles)
	{
		mTransform->SetLocalEulerAngles(eulerAngles);

		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
			child->InitializeTransformMatrices();
	}

	void Node::SetScale(float* scale)
	{
		mTransform->SetLocalScale(scale);

		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
			child->InitializeTransformMatrices();
	}

	void Node::SetScale(const Vector3& scale)
	{
		mTransform->SetLocalScale(scale);

		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
			child->InitializeTransformMatrices();
	}

	void Node::AddChild(Node* child)
	{
		child->mParentNode = this; 
		mChildren.push_back(child);
		TS_CORE_INFO(child->GetName() + " is set as child of " + this->GetName());

		child->UpdateSiblings();
	}

	void Node::RemoveChild(Node* child)
	{
		mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), child), mChildren.end());
		child->UpdateSiblings();
	}

	void Node::RemoveAllChildren()
	{
		for (auto& child : mChildren)
		{
			//child.reset();
			delete &child;
		}

		mChildren.clear();
	}

	Node* Node::GetChildAt(uint32_t childIndex) const
	{
		try
		{
			Node* child = mChildren[childIndex];
			return child;
		}
		catch (std::out_of_range& e)
		{
			TS_CORE_ERROR(e.what());
		}
	}
	
	void Node::UpdateSiblings()
	{
		if (mParentNode)
		{
			mSiblings = {};

			for (auto child : mParentNode->mChildren)
			{
				if (child != this)
					mSiblings.push_back(child);
			}
		}
		else
		{
			TS_CORE_ERROR("There is not parent for " + this->GetName());
		}
	}

	void Node::InitializeTransformMatrices()
	{
		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
		{
			child->InitializeTransformMatrices();
		}
	}

	void Node::UpdateTransformationMatrices(Matrix4 transformationMatrix)
	{
		mTransform->SetTransformationMatrix(transformationMatrix);

		for (auto& child : mChildren)
		{
			child->InitializeTransformMatrices();
		}
	}

	//If there is no parent set parentTransformModelMatrix to identity
	void Node::Update(Ref<Shader> shader, float deltaTime)
	{
		//Send modelMatrix to shader
		shader->SetMat4("u_Model", mTransform->GetTransformationMatrix());

		//Draw Meshes
		for (auto& mesh : mMeshes)
		{
			mesh->Render(mEntity->GetEntityID());
		}

		//Send children modelMatrix to shader and draw gameobject with attached to child
		for (auto& child : mChildren)
		{
			child->Update(shader, deltaTime);
		}
	}

	void Node::LookAt(Ref<Node> targetNode)
	{
		mTransform->LookAt(mParentNode, targetNode->GetTransform());
	}
}