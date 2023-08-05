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

	/*void SceneManager::Initialize()
	{
		if (mCurrentScene)
		{
			mCurrentScene->Initialize();
		}
	}*/

	/*void SceneManager::Update(float deltaTime)
	{
		if (mCurrentScene)
		{
			mCurrentScene->Update(deltaTime);
		}
	}*/

	/*void SceneManager::Render(ShaderManager::ShaderProgram* shaderProgram, const Matrix4& viewMatrix, const Matrix4& projectionMatrix)
	{
		if (mCurrentScene)
		{
			mCurrentScene->Render(shaderProgram, viewMatrix, projectionMatrix);
		}
	}*/

	/*bool SceneManager::HasSiblingWithSameName(Node* parentNode, const char* name)
	{
		for (auto i : parentNode->GetChildren())
		{
			if(strcmp(i->GetName(), name) == 0)
				return true;
		}
		return false;
	}*/

	void SceneManager::SaveCurrentScene()
	{
		if (mCurrentScene)		
			mSceneSerializer->Save(mCurrentScene);		
		else		
			TS_CORE_ERROR("Current scene is not set!");		
	}
}