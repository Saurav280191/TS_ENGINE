#include "tspch.h"
#include "SceneManager/Node.h"
#include "Core/Factory.h"

#ifdef TS_ENGINE_EDITOR
#include <imgui.h>
#endif // TS_ENGINE_EDITOR

// Returns a smart pointer of this node
#define NodeRef std::static_pointer_cast<Node>(shared_from_this())

namespace TS_ENGINE
{
	Node::Node() :
		mType(NodeType::EMPTY),
		mId(0),
		mName(""),
		//mTransform(nullptr),
		mRootNodeNodeId(-1),
		mEnabled(true),
		mIsInitialized(false),
		mParentNode(nullptr),
		mChildren({}),
		//mIsVisibleInEditor(true),
		mSiblings({}),
		mBone(nullptr),
		mMeshes({}),
		mModelPath(""),
		mSceneCamera(nullptr),
		mHasBoneInfluence(false),
		mAnimations({}),
		mCurrentAnimation(nullptr)
	{
		mTransform = CreateRef<Transform>();

#ifdef TS_ENGINE_EDITOR
		mIsVisibleInEditor = true;
#endif
	}

	Node::~Node()
	{
		TS_CORE_INFO("Deleting node with id: {0}, name: {1}", mId, mName);
		
		for (auto& child : mChildren)
			child = nullptr;

		//Destroy();
	}

	void Node::Initialize(const std::string& name, const NodeType& _type)
	{
		mName = name;									// Set Name
		mType = _type;									// Set Type

		SceneManager::GetInstance()->Register(NodeRef);	// Register the current node as a shared pointer in the scene
		
		ComputeTransformMatrices();						// Computer transformation matrices for this node
		mIsInitialized = true;							// Set IsInitialized
	}

	bool Node::CheckSelectedModelRootNodeId(int _selectedModelRootNodeId)
	{
		if (mRootNodeNodeId == _selectedModelRootNodeId)
		{
			return true;
		}

		for (auto child : mChildren)
		{
			child->CheckSelectedModelRootNodeId(_selectedModelRootNodeId);
		}

		return false;
	}

	// If there is no parent set parentTransformModelMatrix to identity
	void Node::Update(Ref<Shader> shader, float deltaTime)
	{
		TS_CORE_ASSERT(mIsInitialized, "Node is not initialized!");

		// Send ModelMatrix to vertex shader
		shader->SetMat4("u_Model",								// Pass worldTransformationMatrix to shader 
			mTransform->GetWorldTransformationMatrix());

		// Set selected modelCopyId
		shader->SetInt("u_ModelRootNodeId", mRootNodeNodeId);	// Pass modelCopyId to shader

		if (mBone)
		{
			mBone->Update(shader);
		}

#ifdef TS_ENGINE_EDITOR
		if (mEnabled)
#endif
		{
			// Draw Meshes
			for (auto& mesh : mMeshes)
			{
#ifdef TS_ENGINE_EDITOR
				mesh->Render(mId, Application::GetInstance().IsTextureModeEnabled());
#else
				mesh->Render(Application::GetInstance().IsTextureModeEnabled());
#endif
			}

			// Send children modelMatrix to shader and draw gameobject with attached to child
			for (auto& child : mChildren)
			{
				child->Update(shader, deltaTime);
			}
		}
	}

	void Node::AnimationUpdate(float _deltaTime)
	{
		// Animation Update
		mCurrentAnimation ? mCurrentAnimation->Update(_deltaTime) : void();

		for (auto& child : mChildren)
		{
			child->AnimationUpdate(_deltaTime);
		}
	}

	void Node::Destroy()
	{
		TS_CORE_INFO("Deleting node named: {0}", mName.c_str());
		SceneManager::GetInstance()->Deregister(mId);

#ifdef TS_ENGINE_EDITOR
		mEnabled = false;
#endif
		if(mParentNode)
			mParentNode->RemoveChild(NodeRef);

		mMeshes.clear();
		mModelPath = "";
	}

	Ref<Node> Node::Duplicate()
	{
		Ref<Node> duplicateNode = CreateRef<Node>();

		Ref<Transform> duplicateNodeTransform = CreateRef<Transform>();				// Transform
		{
			duplicateNodeTransform->mLocalPosition = mTransform->mLocalPosition;
			duplicateNodeTransform->mLocalRotation = mTransform->mLocalRotation;
			duplicateNodeTransform->mLocalScale = mTransform->mLocalScale;

			duplicateNode->mTransform = duplicateNodeTransform;
		}

		for (const auto& mesh : mMeshes)											// Meshes
		{
			duplicateNode->CloneMesh(mesh);
		}

		duplicateNode->SetHasBoneInfluence(HasBoneInfluence());						// HasBoneInfluence

		duplicateNode->mModelPath = mModelPath;										// Model Path

#ifdef TS_ENGINE_EDITOR
		duplicateNode->mIsVisibleInEditor = mIsVisibleInEditor;						// Visibility
#endif

		duplicateNode->Initialize(mName, mType);									// Initialize
		duplicateNode->UpdateSiblings();											// Update Siblings To Set Order In Hierarchy

		duplicateNode->mSceneCamera = mSceneCamera;									// Scene Camera

		for (auto& child : mChildren)												// Children Nodes
		{
			duplicateNode->AddChild(child->Duplicate());								
		}

		return duplicateNode;
	}

	void Node::CloneMesh(const Ref<Mesh>& _originalMesh)
	{
		Ref<Mesh> clonedMesh = CreateRef<Mesh>();
		clonedMesh->CloneMesh(_originalMesh);
		AddMesh(clonedMesh);
	}

	void Node::SetParent(Ref<Node> parentNode)
	{
		if (parentNode)
		{
			TS_CORE_INFO("Changed parent of {0} to {1}", mName.c_str(), parentNode->mName.c_str());
			
			// Set node from current parent
			mParentNode ? mParentNode->RemoveChild(NodeRef) : void();

			// Add node to new parent
			parentNode->AddChild(NodeRef);

			// Computer transforms after parent change
			mTransform->ComputeTransformationMatrix(parentNode);
		}
	}

	void Node::ChangeParent(Ref<Node> parentNode)
	{
		TS_CORE_INFO("Changed parent of {0} to {1}", mName.c_str(), parentNode->mName.c_str());

		if (parentNode)
		{
			// Set node from current parent
			mParentNode ? mParentNode->RemoveChild(NodeRef) : void();

			// Add node to new parent
			parentNode->AddChild(NodeRef);
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

	void Node::SetLocalTransform(const Ref<Transform> _transform)
	{
		mTransform->SetLocalTransform(_transform->GetLocalPosition(), _transform->GetLocalRotation(), _transform->GetLocalScale(), mParentNode);
	}

	void Node::SetSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		mSceneCamera = sceneCamera;
	}

	void Node::AddChild(Ref<Node> _child)
	{
		_child->mParentNode = NodeRef;
		mChildren.push_back(_child);
		//TS_CORE_INFO("{0} is set as child of {1}", _child->mName.c_str(), mName.c_str());

		_child->UpdateSiblings();
	}

	void Node::RemoveChild(Ref<Node> _child)
	{
		TS_CORE_INFO("Removing {0} from parent {1}", _child->mName, mName);
		mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), _child), mChildren.end());
		_child->UpdateSiblings();
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
				TS_CORE_ERROR("Sibling index for: {0} is: {1}", node->mName.c_str(), i);
				return i;
			}
		}

		TS_CORE_ERROR("Could not find sibling index for: {0}", mName.c_str());
		return 0;
	}

	int Node::GetModelRootNodeId()
	{
		return mRootNodeNodeId;
	}

	void Node::SetName(std::string& _name)
	{
		mName = _name;
	}

	void Node::SetBone(Ref<Bone> _bone)
	{
		mBone = _bone;
	}

	void Node::SetHasBoneInfluence(bool _hasBoneInfluence)
	{
		mHasBoneInfluence = _hasBoneInfluence;
	}

	void Node::AddAnimation(Ref<Animation>& _animation)
	{
		mAnimations.insert({ _animation->GetName(), _animation });
	}

	void Node::SetAnimations(const std::unordered_map<std::string, Ref<Animation>>& _animations)
	{
		mAnimations = _animations;
	}

	void Node::SetCurrentAnimation(int _index)
	{
		int i = 0;

		auto& it = mAnimations.begin();
		if (i < _index)
		{
			it++;
			i++;
		}

		mCurrentAnimation = (*it).second;
	}

	void Node::SetCurrentAnimation(std::string _name)
	{
		mCurrentAnimation = mAnimations[_name];		
	}

	void Node::SetModelRootNodeId(int _rootNodeId)
	{
		//std::string name = mName;
		mRootNodeNodeId = _rootNodeId;

		TS_CORE_INFO("Setting mRootNodeId = {0} for {1}", mRootNodeNodeId, mName);

		for (auto& child : mChildren)
		{
			child->SetModelRootNodeId(_rootNodeId);
		}
	}

	void Node::UpdateSiblings()
	{
		if (mParentNode)
		{
			mSiblings = {};

			for (auto& child : mParentNode->mChildren)
			{
				if (child != NodeRef)
					mSiblings.push_back(child);
			}
		}
		else
		{
			TS_CORE_ERROR("There is no parent for {0}", mName.c_str());
		}
	}

	void Node::SetSiblingIndex(int index)
	{
		if (mParentNode)
		{
			int siblingIndex = mParentNode->GetSiblingIndex(NodeRef);

			if (siblingIndex == index)
			{				
				TS_CORE_INFO("Sibling index for: {0} is already {1}", mName.c_str(), index);
			}
			else
			{
				auto elementToMove = mChildren[index];
				mChildren.erase(mChildren.begin() + siblingIndex);
				mChildren.insert(mChildren.begin() + index, elementToMove);
			}
		}
		else
		{
			TS_CORE_ERROR("There is no parent!");
		}
	}

	void Node::ComputeTransformMatrices()
	{
		if(!mHasBoneInfluence)
		{
			mTransform->ComputeTransformationMatrix(mParentNode);
		}		

		for (auto& child : mChildren)
		{
			child->ComputeTransformMatrices();
		}
	}

	void Node::ReInitializeTransforms()
	{
		ComputeTransformMatrices();
	}

	const Ref<Node> Node::FindNodeByName(std::string _name)
	{
		Ref<Node> foundNode = nullptr;

		if (_name == mName)
		{
			foundNode = NodeRef;
		}
		else
		{
			for (auto& childNode : mChildren)
			{
				foundNode = childNode->FindNodeByName(_name);

				if (foundNode)
					break;
			}
		}

		if (!foundNode)
		{
			//TS_CORE_ERROR("Could not find node with name: {0}", _name);
			return nullptr;
		}
		else
		{
			return foundNode;
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
		Factory::GetInstance()->ChangeMeshForNode(NodeRef, primitiveType);
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
		TS_CORE_TRACE("Node {0} has children named: ", mName.c_str());

		for (auto& child : mChildren)
		{
			TS_CORE_TRACE("{0} ", child->mName.c_str());
			child->PrintChildrenName();
		}
	}

	void Node::PrintLocalPosition()
	{
		TS_CORE_INFO("{0}'s LocalPosition = {1}, {2}, {3}", mName.c_str(),
			mTransform->mLocalPosition.x, mTransform->mLocalPosition.y, mTransform->mLocalPosition.z);
	}

	void Node::PrintLocalEulerAngles()
	{
		TS_CORE_INFO("{0}'s LocalEulerAngles = {1}, {2}, {3}", mName.c_str(),
			mTransform->GetLocalEulerAngles().x, mTransform->GetLocalEulerAngles().y, mTransform->GetLocalEulerAngles().z);
	}

	void Node::PrintLocalScale()
	{
		TS_CORE_INFO("{0}'s LocalScale = {1}, {2}, {3}", mName.c_str(),
			mTransform->mLocalScale.x, mTransform->mLocalScale.y, mTransform->mLocalScale.z);
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

	std::string Node::GetNodeTypeStr(const NodeType& _nodeType)
	{
		const char* typeStr = "";

		switch (_nodeType)
		{
		case NodeType::PRIMITIVE:
			typeStr = "PRIMITIVE";
			break;
		case NodeType::MESH:
			typeStr = "MESH";
			break;
		case NodeType::MODELROOTNODE:
			typeStr = "MODELROOTNODE";
			break;
		case NodeType::BONE:
			typeStr = "BONE";
			break;
		case NodeType::BONEGUI:
			typeStr = "BONEGUI";
			break;
		case NodeType::CAMERA:
			typeStr = "CAMERA";
			break;
		case NodeType::SKYBOX:
			typeStr = "SKYBOX";
			break;
		case NodeType::SCENE:
			typeStr = "SCENE";
			break;
		case NodeType::LIGHT:
			typeStr = "LIGHT";
			break;
		case NodeType::EMPTY:
			typeStr = "EMPTY";
			break;
		}
		
		return typeStr;
	}
}