#include "tspch.h"
#include "SceneManager/Node.h"
#include "Core/Factory.h"

#ifdef TS_ENGINE_EDITOR
#include <imgui.h>
#endif // TS_ENGINE_EDITOR

namespace TS_ENGINE
{
	Node::Node()
	{
		this->mIsInitialized = false;
		this->mTransform = CreateRef<Transform>();
		this->mParentNode = nullptr;
		this->mMeshes = {};
		this->mModelPath = "";
#ifdef TS_ENGINE_EDITOR
		this->mIsVisibleInEditor = true;
#endif
		this->mSceneCamera = nullptr;
	}

	Node::~Node()
	{
		Destroy();
	}

	void Node::Destroy()
	{
		TS_CORE_INFO("Deleting node named: {0}", mNodeRef->GetEntity()->GetName().c_str());
		EntityManager::GetInstance()->Remove(mNodeRef->GetEntity()->GetEntityID());

#ifdef TS_ENGINE_EDITOR
		m_Enabled = false;
#endif

		mParentNode->RemoveChild(mNodeRef);

		mMeshes.clear();
		this->mModelPath = "";

		mTransform.reset();

		for (auto& child : mChildren)
		{
			child.reset();
		}

		mParentNode.reset();
		mChildren.clear();
		mSceneCamera = nullptr;
	}

	Ref<Node> Node::Duplicate()
	{
		Ref<Node> duplicateNode = CreateRef<Node>();
		duplicateNode->mNodeRef = duplicateNode;
		duplicateNode->mNodeRef->CloneMeshes(mNodeRef->mMeshes);
		duplicateNode->mNodeRef->mModelPath = mNodeRef->mModelPath;

		duplicateNode->mNodeRef->mTransform = CreateRef<Transform>();
		duplicateNode->mNodeRef->mTransform->m_Pos = mNodeRef->mTransform->m_Pos;
		duplicateNode->mNodeRef->mTransform->m_EulerAngles = mNodeRef->mTransform->m_EulerAngles;
		duplicateNode->mNodeRef->mTransform->m_Scale = mNodeRef->mTransform->m_Scale;

#ifdef TS_ENGINE_EDITOR
		duplicateNode->mNodeRef->mIsVisibleInEditor = mNodeRef->mIsVisibleInEditor;
#endif

		duplicateNode->mParentNode = mNodeRef->mParentNode;

		for (auto& child : mNodeRef->mChildren)
		{
			duplicateNode->AddChild(child->Duplicate());
		}

		duplicateNode->mNodeRef->Initialize(mNodeRef->mEntity->GetName(), mNodeRef->mEntity->GetEntityType());
		duplicateNode->mNodeRef->UpdateSiblings();

		duplicateNode->mNodeRef->mSceneCamera = mNodeRef->mSceneCamera;

		return duplicateNode;
	}

	/*void Node::SetEntityType(EntityType entityType)
	{
		mEntityType = entityType;
	}*/

	void Node::SetNodeRef(Ref<Node> node)
	{
		mNodeRef = node;
	}

	void Node::SetParent(Ref<Node> parentNode)
	{
		//TS_CORE_INFO("Setting parent of {0} as {1}", mEntity->GetName().c_str(), parentNode->mEntity->GetName().c_str());

		if (parentNode)
		{
			if (mNodeRef->mParentNode)
			{
				mNodeRef->mParentNode->RemoveChild(mNodeRef);
			}

			parentNode->AddChild(mNodeRef);
			mNodeRef->GetTransform()->ComputeTransformationMatrix(parentNode);
		}
	}

	void Node::ChangeParent(Ref<Node> parentNode)
	{
		TS_CORE_INFO("Changed parent of {0} to {1}", mEntity->GetName().c_str(), parentNode->mEntity->GetName().c_str());

		if (parentNode)
		{
			if (mNodeRef->mParentNode)
			{
				mNodeRef->mParentNode->RemoveChild(mNodeRef);
			}

			parentNode->AddChild(mNodeRef);
		}
	}

	void Node::SetPosition(float* pos)
	{
		mTransform->SetLocalPosition(pos);
		InitializeTransformMatrices();
	}
	void Node::SetPosition(float x, float y, float z)
	{
		mTransform->SetLocalPosition(x, y, z);
		InitializeTransformMatrices();
	}
	void Node::SetPosition(const Vector3& pos)
	{
		mTransform->SetLocalPosition(pos);
		InitializeTransformMatrices();
	}

	void Node::SetEulerAngles(float* eulerAngles)
	{
		mTransform->SetLocalEulerAngles(eulerAngles);
		InitializeTransformMatrices();
	}
	void Node::SetEulerAngles(float x, float y, float z)
	{
		mTransform->SetLocalEulerAngles(x, y, z);
		InitializeTransformMatrices();
	}
	void Node::SetEulerAngles(const Vector3& eulerAngles)
	{
		mTransform->SetLocalEulerAngles(eulerAngles);
		InitializeTransformMatrices();
	}

	void Node::SetScale(float* scale)
	{
		mTransform->SetLocalScale(scale);
		InitializeTransformMatrices();
	}
	void Node::SetScale(float x, float y, float z)
	{
		mTransform->SetLocalScale(x, y, z);
		InitializeTransformMatrices();
	}
	void Node::SetScale(const Vector3& scale)
	{
		mTransform->SetLocalScale(scale);
		InitializeTransformMatrices();
	}

	void Node::SetSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		mSceneCamera = sceneCamera;
	}

	void Node::AddChild(Ref<Node> child)
	{
		child->mParentNode = mNodeRef;
		mChildren.push_back(child);
		//TS_CORE_INFO("{0} is set as child of {1}", child->mEntity->GetName().c_str(), mNodeRef->mEntity->GetName().c_str());

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
			delete& child;
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

	const int Node::GetSiblingIndex(Ref<Node> node)
	{
		for (int i = 0; i < mChildren.size(); i++)
		{
			if (mChildren[i] == node)
			{
				TS_CORE_ERROR("Sibling index for: {0} is: {1}", node->GetEntity()->GetName().c_str(), i);
				return i;
			}
		}

		TS_CORE_ERROR("Could not find sibling index for: {0}", node->GetEntity()->GetName().c_str());
		return 0;
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
			TS_CORE_ERROR("There is no parent for {0}", mNodeRef->mEntity->GetName().c_str());
		}
	}

	void Node::SetSiblingIndex(int index)
	{
		if (mParentNode)
		{
			int siblingIndex = mParentNode->GetSiblingIndex(mNodeRef);

			if (siblingIndex == index)
			{
				TS_CORE_INFO("Sibling index for: {0} is already {1}", mNodeRef->GetEntity()->GetName().c_str(), index);
			}
			else
			{
				auto elementToMove = mNodeRef->mChildren[index];
				mNodeRef->mChildren.erase(mNodeRef->mChildren.begin() + siblingIndex);
				mNodeRef->mChildren.insert(mNodeRef->mChildren.begin() + index, elementToMove);
			}
		}
		else
		{
			TS_CORE_ERROR("There is no parent!");
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
		//mTransform->m_GlobalTransformationMatrix = transformationMatrix;

		for (auto& child : mChildren)
		{
			child->InitializeTransformMatrices();
		}
	}

	/// <summary>
	/// Sets name and registers entityType
	/// </summary>
	/// <param name="name"></param>
	/// <param name="entityType"></param>
	void Node::Initialize(const std::string& name, const EntityType& entityType)
	{
		mNodeRef->mEntity = EntityManager::GetInstance()->Register(name, entityType);
		InitializeTransformMatrices();

		mIsInitialized = true;
	}

	void Node::ReInitializeTransforms()
	{
		InitializeTransformMatrices();
	}

	// If there is no parent set parentTransformModelMatrix to identity
	void Node::Update(Ref<Shader> shader, float deltaTime)
	{
		TS_CORE_ASSERT(mIsInitialized, "Node is not initialized!");

		// Send ModelMatrix to vertex shader
		shader->SetMat4("u_Model", mTransform->GetGlobalTransformationMatrix());

#ifdef TS_ENGINE_EDITOR
		if (m_Enabled)
#endif
		{
			// Draw Meshes
			for (auto& mesh : mMeshes)
			{
#ifdef TS_ENGINE_EDITOR
				mesh->Render(mEntity->GetEntityID());
#else
				mesh->Render();
#endif
			}

			// Send children modelMatrix to shader and draw gameobject with attached to child
			for (auto& child : mChildren)
			{
				child->Update(shader, deltaTime);
			}
		}
	}

	void Node::LookAt(Ref<Node> targetNode)
	{
		mTransform->LookAt(mParentNode, targetNode->GetTransform());
	}

	void Node::ReplaceMesh(Ref<Mesh> mesh)
	{
		mMeshes.clear();
		mMeshes.push_back(mesh);
	}

	void Node::ChangeMesh(PrimitiveType primitiveType)
	{
		Factory::GetInstance()->ChangeMeshForNode(mNodeRef, primitiveType);
	}

	void Node::AddMesh(Ref<Mesh> mesh)
	{
		mMeshes.push_back(mesh);
	}

	void Node::AddMeshes(std::vector<Ref<Mesh>> meshes)
	{
		mMeshes = meshes;
	}

	void Node::RemoveAllMeshes()
	{
		mMeshes.clear();
	}

	bool Node::HasMeshes()
	{
		return mMeshes.size() > 0 ? true : false;
	}

	void Node::SetModelPath(std::string modelPath)
	{
		mModelPath = modelPath;
	}

	void Node::PrintChildrenName()
	{
		TS_CORE_TRACE("Node {0} has children named: ", mEntity->GetName().c_str());

		for (auto& child : mChildren)
		{
			TS_CORE_TRACE("{0} ", child->mEntity->GetName().c_str());
			child->PrintChildrenName();
		}
	}

	void Node::CloneMeshes(std::vector<Ref<Mesh>> meshes)
	{
		for (auto mesh : meshes)
		{
			Ref<Mesh> clonedMesh = CreateRef<Mesh>();
			clonedMesh->CloneMesh(mesh);
			mNodeRef->AddMesh(clonedMesh);
		}

	}

#ifdef TS_ENGINE_EDITOR
	void Node::HideInEditor()
	{
		mIsVisibleInEditor = false;
	}
#endif
}