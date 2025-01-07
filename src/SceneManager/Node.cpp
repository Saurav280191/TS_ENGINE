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
		duplicateNode->mNodeRef->mTransform->mLocalPosition = mNodeRef->mTransform->mLocalPosition;
		duplicateNode->mNodeRef->mTransform->mLocalRotation = mNodeRef->mTransform->mLocalRotation;
		duplicateNode->mNodeRef->mTransform->mLocalScale = mNodeRef->mTransform->mLocalScale;

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

	void Node::SetPosition(aiVector3D _assimpPosition)
	{
		mTransform->SetLocalPosition(_assimpPosition.x, _assimpPosition.y, _assimpPosition.z);
		ComputeTransformMatrices();
	}
	void Node::SetPosition(float* pos)
	{
		mTransform->SetLocalPosition(pos);
		ComputeTransformMatrices();
	}
	void Node::SetPosition(float x, float y, float z)
	{
		mTransform->SetLocalPosition(x, y, z);
		ComputeTransformMatrices();
	}
	void Node::SetPosition(const Vector3& pos)
	{
		mTransform->SetLocalPosition(pos);
		ComputeTransformMatrices();
	}

	void Node::SetEulerAngles(aiVector3D _assimpEulerAngles)
	{
		mTransform->SetLocalEulerAngles(Vector3(_assimpEulerAngles.x, _assimpEulerAngles.y, _assimpEulerAngles.z));
		ComputeTransformMatrices();
	}
	void Node::SetEulerAngles(float* eulerAngles)
	{
		mTransform->SetLocalEulerAngles(Vector3(eulerAngles[0], eulerAngles[1], eulerAngles[2]));
		ComputeTransformMatrices();
	}
	void Node::SetEulerAngles(float x, float y, float z)
	{
		mTransform->SetLocalEulerAngles(Vector3(x, y, z));
		ComputeTransformMatrices();
	}
	void Node::SetEulerAngles(const Vector3& eulerAngles)
	{
		mTransform->SetLocalEulerAngles(eulerAngles);
		ComputeTransformMatrices();
	}

	void Node::SetRotation(aiQuaternion _rotation)
	{
		mTransform->mLocalRotation = Quaternion(_rotation.w, _rotation.x, _rotation.y, _rotation.z);
	}

	void Node::SetScale(aiVector3D _assimpScale)
	{
		mTransform->SetLocalScale(_assimpScale.x, _assimpScale.y, _assimpScale.z);
		ComputeTransformMatrices();
	}
	void Node::SetScale(float* scale)
	{
		mTransform->SetLocalScale(scale);
		ComputeTransformMatrices();
	}
	void Node::SetScale(float x, float y, float z)
	{
		mTransform->SetLocalScale(x, y, z);
		ComputeTransformMatrices();
	}
	void Node::SetScale(const Vector3& scale)
	{
		mTransform->SetLocalScale(scale);
		ComputeTransformMatrices();
	}

	void Node::SetLocalTransform(Vector3 _localPosition, Vector3 _localEulerAngles, Vector3 _localScale)
	{
		mTransform->SetLocalTransform(_localPosition, _localEulerAngles, _localScale, mParentNode);
	}

	void Node::SetLocalTransform(Vector3 _localPosition, Quaternion _localQuaternion, Vector3 _localScale)
	{
		mTransform->SetLocalTransform(_localPosition, _localQuaternion, _localScale, mParentNode);
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

	void Node::ComputeTransformMatrices()
	{
		mTransform->ComputeTransformationMatrix(mParentNode);

		for (auto& child : mChildren)
		{
			child->ComputeTransformMatrices();
		}
	}

	/// <summary>
	/// Sets name and registers entityType
	/// </summary>
	/// <param name="name"></param>
	/// <param name="entityType"></param>
	void Node::Initialize(const std::string& name, const EntityType& entityType)
	{
		mName = name;
		mNodeRef->mEntity = EntityManager::GetInstance()->Register(name, entityType);
		ComputeTransformMatrices();

		mIsInitialized = true;
	}

	void Node::ReInitializeTransforms()
	{
		ComputeTransformMatrices();
	}

	// If there is no parent set parentTransformModelMatrix to identity
	void Node::Update(Ref<Shader> shader, float deltaTime)
	{
		TS_CORE_ASSERT(mIsInitialized, "Node is not initialized!");

		// Send ModelMatrix to vertex shader
		shader->SetMat4("u_Model", mTransform->GetWorldTransformationMatrix());

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

	void Node::PrintTransform()
	{
		TS_CORE_INFO("{0}'s LocalPosition = {1}, {2}, {3}, LocalEulerAngles = {4}, {5}, {6}, LocalScale = {7}, {8}, {9}", mName.c_str(),
			mTransform->mLocalPosition.x, mTransform->mLocalPosition.y, mTransform->mLocalPosition.z,
			mTransform->GetLocalEulerAngles().x, mTransform->GetLocalEulerAngles().y, mTransform->GetLocalEulerAngles().z, 
			mTransform->mLocalScale.x, mTransform->mLocalScale.y, mTransform->mLocalScale.z);
	}

#ifdef TS_ENGINE_EDITOR
	void Node::HideInEditor()
	{
		mIsVisibleInEditor = false;
	}
#endif
}