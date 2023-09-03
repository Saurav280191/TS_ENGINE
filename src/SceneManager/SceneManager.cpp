#include "tspch.h"
#include "SceneManager/SceneManager.h"

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
		mCurrentScene(NULL)
	{
		
	}

	SceneManager::~SceneManager()
	{
		if (mCurrentScene)
		{
			mCurrentScene->~Scene();
			mCurrentScene = nullptr;
		}

		mInstance = nullptr;
		delete mInstance;
	}

	void SceneManager::SetCurrentScene(Ref<Scene> scene)
	{
		mCurrentScene = scene;
	}

	Ref<Scene> SceneManager::GetCurrentScene()
	{
		return mCurrentScene;
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
		Ref<EditorCamera> editorCamera = CreateRef<EditorCamera>("EditorCamera");
		editorCamera->SetPerspective(TS_ENGINE::Camera::Perspective(60.0f, 1.77f, 0.1f, 1000.0f));
		editorCamera->GetNode()->GetTransform()->SetLocalPosition(-0.738f, 5.788f, 14.731f);
		editorCamera->GetNode()->GetTransform()->SetLocalEulerAngles(-18.102f, 0.066f, 0.0f);
		editorCamera->CreateFramebuffer(1920, 1080);//Create framebuffer for editorCamera
		editorCamera->Initialize();
		editorCamera->GetNode()->InitializeTransformMatrices();

		Ref<Scene> scene = CreateRef<Scene>(sceneName, editorCamera);

		mCurrentScene = scene;
	}

	void SceneManager::SaveCurrentScene()
	{
		if (mCurrentScene)
		{
			mSceneSerializer->Save(mCurrentScene);	
		}
		else
		{
			TS_CORE_ERROR("Current scene is not set!");
		}
	}
}