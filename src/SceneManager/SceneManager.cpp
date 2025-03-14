#include "tspch.h"
#include "SceneManager.h"
#include "Factory.h"
#include "Utils/Utility.h"
#include "Renderer/MaterialManager.h"

namespace TS_ENGINE
{
	SceneManager* SceneManager::mInstance = NULL;

	SceneManager* SceneManager::GetInstance()
	{
		if (!mInstance)
			mInstance = new SceneManager();

		return mInstance;
	}

	SceneManager::SceneManager() :
		mCurrentScene(NULL),
		mNodeMap({}),
		mFreeIds(std::queue<uint32_t>()),
		mNextId(1)
	{
		
	}

	SceneManager::~SceneManager()
	{
		mSceneSerializer = nullptr;

		if (mCurrentScene)
		{
			mCurrentScene = nullptr;
		}

		mInstance = nullptr;
		delete mInstance;
	}

	void SceneManager::SetCurrentScene(const Ref<Scene>& scene)
	{
		mCurrentScene = scene;
	}

	Ref<Scene>& SceneManager::GetCurrentScene()
	{
		return mCurrentScene;
	}

	void SceneManager::ClearCurrentScene()
	{
		if(mCurrentScene)
		{
			mCurrentScene->Destroy();
			mCurrentScene = nullptr;
		}

		if (mNodeMap.size() > 0)
		{
			// Deregister all nodes
			while(mNodeMap.size() > 0)
			{
				auto it = mNodeMap.begin();
				Deregister(it->first);
			}
		}

		// Clear mNodeMaps 
		mNodeMap.clear();
		// Clear FreeIds 
		mFreeIds = std::queue<uint32_t>();
		// Set mNextId to default value 
		mNextId = 1;
	}

	Ref<Node> SceneManager::GetCurrentSceneNode()
	{
		if (mCurrentScene)
			return mCurrentScene->GetSceneNode();
		else
		{
			TS_CORE_ERROR("Current scene is not set!");
			return nullptr;
		}
	}

	void SceneManager::CreateNewScene(const std::string& sceneName)
	{
		// Create scene for editor or sandbox
		Ref<Scene> scene = CreateRef<Scene>();																// Create Scene
		scene->SetName(sceneName);																			// Set Scene Name

		Ref<Skybox> skybox = CreateRef<Skybox>();															// Create skybox
		skybox->Initialize("Skybox", NodeType::SKYBOX);
		scene->AttachSkybox(skybox);																		// Attach skybox to scene

		// Create editor and scene camera for Editor or Sandbox
#ifdef TS_ENGINE_EDITOR
		// Initialize editor camera
		Ref<EditorCamera> editorCamera = CreateRef<EditorCamera>("EditorCamera");							// Create Editor Camera
		editorCamera->SetPerspective(TS_ENGINE::Camera::Perspective(60.0f, 1.77f, 0.1f, 1000.0f));			
		editorCamera->GetNode()->GetTransform()->SetLocalPosition(-0.738f, 5.788f, 14.731f);
		editorCamera->GetNode()->GetTransform()->SetLocalEulerAngles(-18.102f, 0.066f, 0.0f);
		editorCamera->CreateFramebuffer(1920, 1080);														// Create Framebuffer For EditorCamera
		editorCamera->Initialize();
		editorCamera->GetNode()->ReInitializeTransforms();

		// Initialize scene camera for editor
		auto sceneCameraNode = Factory::GetInstance()->InstantitateSceneCamera("SceneCamera", editorCamera);// Instantiate Scene Camera For Editor
		sceneCameraNode->GetTransform()->SetLocalPosition(7.156f, 2.951f, 8.770f);
		sceneCameraNode->GetTransform()->SetLocalEulerAngles(-13.235f, 38.064f, 0.0f);
		
		// Add editor camera to scene
		scene->AddEditorCamera(editorCamera);
		// Add scene camera to scene
		scene->AddSceneCamera(sceneCameraNode->GetSceneCamera());											// Add Editor Camera To Scene
#else
		// Initialize editor camera
		Ref<Node> sceneCameraNode = Factory::GetInstance()->InstantitateSceneCamera("SceneCamera");									// Instantiate Scene Camera
		
		Ref<SceneCamera> sceneCamera = sceneCameraNode->GetSceneCamera();
		sceneCamera->SetPerspective(TS_ENGINE::Camera::Perspective(60.0f, 1.77f, 0.1f, 1000.0f));
		sceneCamera->CreateFramebuffer(1920, 1080);																					// Create Framebuffer For Sandbox
		sceneCamera->Initialize();
		//sceneCameraNode->GetTransform()->SetLocalPosition(7.156f, 2.951f, 8.770f);
		//sceneCameraNode->GetTransform()->SetLocalEulerAngles(-13.235f, 38.064f, 0.0f);
		sceneCameraNode->GetTransform()->SetLocalPosition(-5.24730539f, 7.7407055f, -6.37276411f);
		sceneCameraNode->GetTransform()->SetLocalEulerAngles(-15.0f, 225.0f, 0);
		sceneCameraNode->ComputeTransformMatrices();

		// Add scene camera to scene
		scene->AddSceneCamera(sceneCamera);																							// Add Scene Camera to Scene

		// Instantiate test model for Sandbox
		Ref<Node> modelNode = nullptr;

		std::string modelName = "Ely By K.Atienza.fbx";
		//std::string modelName = "Y Bot.fbx";

		if (TS_ENGINE::Utility::FileExists("..//..//..//Assets//Models//" + modelName))
			modelNode = Factory::GetInstance()->InstantiateModel("..//..//..//Assets//Models//" + modelName, scene->GetSceneNode()).first;// Instantiate Model
		else if (TS_ENGINE::Utility::FileExists("Assets//Models//" + modelName))
			modelNode = Factory::GetInstance()->InstantiateModel("Assets//Models//" + modelName, scene->GetSceneNode()).first;			// Instantiate Model
		else
			TS_CORE_ERROR("Invalid model path!");

		// Rotate leftArm for testing bone influence
		Ref<Node> leftArmNode = scene->GetSceneNode()->FindNodeByName("mixamorig:LeftArm");
		Vector3 leftArmLocalEulerAngles = leftArmNode->GetTransform()->GetLocalEulerAngles();
		leftArmNode->GetTransform()->SetLocalEulerAngles(leftArmLocalEulerAngles.x, leftArmLocalEulerAngles.y , leftArmLocalEulerAngles.z + 45.0f);

		Ref<Node> rightArmNode = scene->GetSceneNode()->FindNodeByName("mixamorig:RightArm");
		Vector3 rightArmLocalEulerAngles = rightArmNode->GetTransform()->GetLocalEulerAngles();
		rightArmNode->GetTransform()->SetLocalEulerAngles(rightArmLocalEulerAngles.x, rightArmLocalEulerAngles.y, rightArmLocalEulerAngles.z - 45.0f);
		
		Ref<Node> spineNode = scene->GetSceneNode()->FindNodeByName("mixamorig:Spine");
		Vector3 spineLocalEulerAngles = spineNode->GetTransform()->GetLocalEulerAngles();
		spineNode->GetTransform()->SetLocalEulerAngles(spineLocalEulerAngles.x, spineLocalEulerAngles.y + 45.0f, spineLocalEulerAngles.z);

		if (modelNode)
		{
			modelNode->GetTransform()->SetLocalScale(0.1f, 0.1f, 0.1f);
			modelNode->ComputeTransformMatrices();
		}

		Ref<Model> model = nullptr;

		if (TS_ENGINE::Utility::FileExists("..//..//..//Assets//Models//" + modelName))
			model = Factory::GetInstance()->mLoadedModelNodeMap["..//..//..//Assets//Models//" + modelName].second;
		else if (TS_ENGINE::Utility::FileExists("Assets//Models//" + modelName))
			model = Factory::GetInstance()->mLoadedModelNodeMap["Assets//Models//" + modelName].second;

		// Set mSelectedModelRootNodeId, mSelectedModelRootNodeId, 
		// and mSelectedBoneId for showing influence on Model
		{
			scene->mSelectedModelRootNodeId = modelNode->GetId();
			Ref<Bone> bone = model->FindBoneByName("mixamorig:LeftArm");
			scene->mSelectedModelRootNodeId = bone->GetNode()->GetModelRootNodeId();
			scene->mSelectedBoneId = bone->GetId();
		}
#endif

		// Default Ground
		Ref<Node> groundNode = Factory::GetInstance()->InstantiateQuad("Ground",		// Instantiate Ground
			scene->GetSceneNode());	
		groundNode->GetTransform()->SetLocalEulerAngles(-90.0f, 0.0f, 0.0f);
		groundNode->GetTransform()->SetLocalScale(10.0f, 10.0f, 10.0f);
		auto& groundMesh = groundNode->GetMesh();
		groundMesh->GetMaterial()->SetAmbientColor(Vector4(0.7f, 0.7f, 0.7f, 1.0f));
		groundNode->ComputeTransformMatrices();

		Ref<Node> terrainNode = Factory::GetInstance()->InstantiateTerrain("Terrain",	// Terrain 
			100.0, 100.0, 0.5, scene->GetSceneNode());		
		//terrainNode->GetMesh()->SetMaterial(MaterialManager::GetInstance()->GetMaterial("Lit"));
		terrainNode->ComputeTransformMatrices();

		scene->Initialize();															// Initialize Scene
		
		mCurrentScene = scene;															// Set Scene As Current Scene
	}

	void SceneManager::SaveCurrentScene()
	{
		if (mCurrentScene)
		{
#ifdef TS_ENGINE_EDITOR
			mSceneSerializer->Save(mCurrentScene);	
#endif
		}
		else
		{
			TS_CORE_ERROR("Current scene is not set!");
		}
	}
	
	void SceneManager::LoadScene(const std::string& savedScenePath)
	{
#ifdef TS_ENGINE_EDITOR
		mSceneSerializer->Load(savedScenePath);
#endif
	}

	uint32_t SceneManager::Register(Ref<Node> _node)
	{
		uint32_t id;

		// Reuse an old ID if available
		if (!mFreeIds.empty())
		{
			id = mFreeIds.front();
			mFreeIds.pop();
		}
		else
		{
			id = mNextId++;
		}

		TS_CORE_INFO("Registered node with Id: {0}, Name: {1}", id, _node->GetName());

		mNodeMap[id] = _node;	// Add node to map
		_node->mId = id;		// Set node Id
		return id;
	}

	void SceneManager::Deregister(uint32_t _id)
	{
		auto it = mNodeMap.find(_id);

		if (it != mNodeMap.end())
		{
			TS_CORE_INFO("Deregistered node with Id: {0}, Name: {1}", _id, it->second->GetName());
			
			// Ref<Node> will be deleted automatically as it is smart pointer
			mNodeMap.erase(it);
			mFreeIds.push(_id);
		}
	}
}