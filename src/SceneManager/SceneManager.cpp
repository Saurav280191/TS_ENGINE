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

	void SceneManager::SaveCurrentScene()
	{
		if (mCurrentScene)		
			mSceneSerializer->Save(mCurrentScene);		
		else		
			TS_CORE_ERROR("Current scene is not set!");
	}
}