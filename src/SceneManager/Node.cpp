#include "tspch.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	Node::Node() 
	{
		this->mName = "";
		this->mTransform = CreateRef<Transform>();
		this->mParentNode = nullptr;
		this->mMeshes = {}; 
#ifdef TS_ENGINE_EDITOR
		this->mIsVisibleInEditor = true;
#endif
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

		for (auto& node : mChildren)
		{
			node.reset();
		}

		mParentNode.reset();

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
	
	void Node::SetNodeRef(Ref<Node> node)
	{
		mNodeRef = node;
	}

	void Node::SetName(const std::string& name) 
	{ 
		//TS_CORE_TRACE("Renamed Node with entityID {0} to {1}", mEntity->GetEntityID(), name);
		mName = name;
		mNodeRef->mEntity = EntityManager::GetInstance()->Register(name);
		//mEntity->UpdateName(name);
	}

	void Node::SetParent(Ref<Node> parentNode)
	{
		TS_CORE_INFO("Setting parent of {0} as {1}", mName.c_str(), parentNode->GetName());

		if (mNodeRef->mParentNode)
		{
			mNodeRef->mParentNode->RemoveChild(mNodeRef);
		}

		parentNode->AddChild(mNodeRef);
		mNodeRef->GetTransform()->ComputeTransformationMatrix(parentNode);
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

	void Node::AddChild(Ref<Node> child)
	{
		child->mParentNode = mNodeRef; 
		mChildren.push_back(child);
		TS_CORE_INFO(child->GetName() + " is set as child of " + mNodeRef->GetName());

		child->UpdateSiblings();
	}

	void Node::RemoveChild(Ref<Node> child)
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

	Ref<Node> Node::GetChildAt(uint32_t childIndex) const
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
	
	void Node::UpdateSiblings()
	{
		if (mParentNode)
		{
			mSiblings = {};

			for (auto& child : mParentNode->mChildren)
			{
				if (child != mNodeRef)
					mSiblings.push_back(child);
			}
		}
		else
		{
			TS_CORE_ERROR("There is not parent for " + mNodeRef->GetName());
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

	void Node::AddMesh(Ref<Mesh> mesh)
	{
		mMeshes.push_back(mesh); 
	}
	
	void Node::AddMeshes(std::vector<Ref<Mesh>> _meshes)
	{
		mMeshes = _meshes;
	}
	
	void Node::PrintChildrenName()
	{
		TS_CORE_TRACE("Node {0} has children named: ", mName.c_str());

		for (auto& child : mChildren)
		{
			TS_CORE_TRACE("{0} ", child->mName.c_str());
			child->PrintChildrenName();
		}
	}
	
	void Node::HideInEditor()
	{
		mIsVisibleInEditor = false;
	}
}