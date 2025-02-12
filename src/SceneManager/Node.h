#pragma once
#include "tspch.h"
#include <memory>
#include "Core/Transform.h"
#include "Renderer/Shader.h"
#include "Primitive/Mesh.h"
#include "Components/Animation.h"

namespace TS_ENGINE
{
	enum NodeType
	{
		PRIMITIVE,
		MESH,
		MODELROOTNODE,
		BONE,
		BONEGUI,
		CAMERA,
		SKYBOX,
		SCENE,
		LIGHT,
		EMPTY
	};

	class Bone;
	class Transform;
	class SceneCamera;
	class Animation;

	// Enable shared_from_this for Node class objects
	class Node	: public std::enable_shared_from_this<Node>
	{		
	public:
		Node();
		~Node();	

		// Registers node and initializes transformation matrix
		virtual void Initialize(const std::string& name, const NodeType& _type);

		// To check mSelectedModelRootNodeId match on CPU
		bool CheckSelectedModelRootNodeId(int _selectedModelRootNodeId);
		
		// Sets model matrix in shader. Renders mesh. Then updates children.
		void Update(Ref<Shader> shader, float deltaTime);
		
		// Animation Update
		void AnimationUpdate(float _deltaTime);

		void Destroy();		
		Ref<Node> Duplicate();

		void CloneMesh(const Ref<Mesh>& _originalMesh);

		// Changes the parent and computes transforms
		void SetParent(Ref<Node> parentNode);

		// Changes the parent without computing transforms
		void ChangeParent(Ref<Node> parentNode);

		void SetPosition(aiVector3D _assimpPosition);
		void SetPosition(float* pos);
		void SetPosition(float x, float y, float z);
		void SetPosition(const Vector3& pos);
		void SetEulerAngles(aiVector3D _assimpEulerAngles);
		void SetEulerAngles(float* eulerAngles);
		void SetEulerAngles(float x, float y, float z);
		void SetEulerAngles(const Vector3& eulerAngles);
		
		void SetRotation(aiQuaternion _rotation);

		void SetScale(aiVector3D _assimpScale);
		void SetScale(float* scale);
		void SetScale(float x, float y, float z);
		void SetScale(const Vector3& scale);

		// Sets local transform in Transform
		void SetLocalTransform(Vector3 _localPosition, Vector3 _localEulerAngles, Vector3 _localScale);
		// Sets local transform in Transform
		void SetLocalTransform(Vector3 _localPosition, Quaternion _localQuaternion, Vector3 _localScale);

		void SetLocalTransform(const Ref<Transform> _transform);

		void SetSceneCamera(Ref<SceneCamera> sceneCamera);

		void AddChild(Ref<Node> _child);
		void RemoveChild(Ref<Node> _child);
		void RemoveAllChildren();
		void UpdateSiblings();
		void SetSiblingIndex(int index);

		// Updates local and global model matrices for itself and for children
		void ComputeTransformMatrices();	
		
		// Initializes transformation matrices of node and it's children with registering node again
		void ReInitializeTransforms();

		const Ref<Node> FindNodeByName(std::string _name);

		void LookAt(Ref<Node> targetNode);

		void ReplaceMesh(Ref<Mesh> mesh);
		void ChangeMesh(PrimitiveType primitiveType);

		void AddMesh(Ref<Mesh> mesh);
		void AddMeshes(std::vector<Ref<Mesh>> meshes);
		void RemoveAllMeshes();
		bool HasMeshes();

		void SetModelPath(std::string modelPath);

		void PrintChildrenName();// Only for testing

		void PrintLocalPosition();
		void PrintLocalEulerAngles();
		void PrintLocalScale();

		void PrintTransform();

#ifdef TS_ENGINE_EDITOR
		void HideInEditor();					
#endif

#pragma region Getters
		const std::string& GetName() { return mName; }
		NodeType GetNodeType() { return mType; }
		std::string GetNodeTypeStr(const NodeType& _nodeType);
		const bool IsVisibleInEditor() const { return mIsVisibleInEditor; }			
		Ref<Node> GetChildAt(uint32_t childIndex) const;
		Ref<Node> GetParentNode() const { return mParentNode; }
		std::vector<Ref<Node>> GetChildren() const { return mChildren; }
		const std::vector<Ref<Node>> GetSiblings() const { return mSiblings; }
		const Ref<Transform> GetTransform() const { return mTransform; }
		const size_t GetChildCount() const { return mChildren.size(); }
		std::vector<Ref<Mesh>> GetMeshes() const { return mMeshes; }
		Ref<SceneCamera> GetSceneCamera() { return mSceneCamera; }
		Ref<Mesh> GetMesh() const { return mMeshes[0]; }
		Ref<Bone> GetBone() { return mBone; }
		std::string GetModelPath() { return mModelPath; }
		const int GetSiblingIndex(Ref<Node> node);
		
		// Finds and returns root node's Id
		int GetModelRootNodeId();

		const Ref<Animation> GetCurrentAnimation() const { return mCurrentAnimation; }
		const std::unordered_map<std::string, Ref<Animation>> GetAnimations() const { return mAnimations; }
#pragma endregion

		void SetName(std::string& _name);
		void SetBone(Ref<Bone> _bone);
		void SetHasBoneInfluence(bool _hasBoneInfluence);
		bool HasBoneInfluence() { return mHasBoneInfluence; }
		void AddAnimation(Ref<Animation>& _animation);
		void SetAnimations(const std::unordered_map<std::string, Ref<Animation>>& _animations);
		void SetCurrentAnimation(int _index);
		void SetCurrentAnimation(std::string _name);
		void SetModelRootNodeId(int _rootNodeId);

	private:
		NodeType mType;							// Node Type
	public:
		uint32_t mId;							// Id. Get's set while being Registered in Scene class
		std::string mName;						// Name
		Ref<Transform> mTransform;				// Transform
		int mRootNodeNodeId;					// Model's root node Id
#ifdef TS_ENGINE_EDITOR
		bool mEnabled = true;					// Enabled
#endif
	private:
		bool mIsInitialized;					// Initialize		
		Ref<Node> mParentNode;					// Parent Node
		std::vector<Ref<Node>> mChildren;		// Children Nodes

#ifdef TS_ENGINE_EDITOR
		bool mIsVisibleInEditor = true;			// Visible(Not In Use. Can be used for Toggle Mesh Visibility)
#endif
		
		std::vector<Ref<Node>> mSiblings = {};	// Siblings

		Ref<Bone> mBone;						// Bone

		std::vector<Ref<Mesh>> mMeshes;			// Meshes
		std::string mModelPath;					// Model Path (Used to save model path in scene serializer)
		Ref<SceneCamera> mSceneCamera;			// Only used incase of scene camera node
		bool mHasBoneInfluence;					// This helps in identifying mesh nodes have bone influence and need to be ignored while computing transforms in ComputeTransformMatrices function
		
		std::unordered_map<std::string, 
			Ref<Animation>> mAnimations;		// Name & Animation Map
		Ref<Animation> mCurrentAnimation;		// Current Animation			
	};
}