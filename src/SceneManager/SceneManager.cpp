#include "tspch.h"
#include "SceneManager/SceneManager.h"

namespace TS_ENGINE
{
	namespace SceneManager
	{
		SceneManager* SceneManager::m_pInstance = NULL;

		SceneManager* SceneManager::GetInstance()
		{
			if (!m_pInstance)
				m_pInstance = new SceneManager();

			return m_pInstance;
		}

		SceneManager::SceneManager():
			mCurrentScene(NULL)
		{
			Logger::Print("[Log]Created scene manager");
		}

		SceneManager::~SceneManager()
		{
			Logger::Print("[Log]Destroying scene manager");
		}

		void SceneManager::SetCurrentScene(Scene* scene)
		{
			//Delete current scene before resetting it
			if (mCurrentScene != NULL)
			{
				mCurrentScene = NULL;
				delete mCurrentScene;
			}

			mCurrentScene = scene;
		}

		Scene* SceneManager::GetCurrentScene()
		{
			return mCurrentScene;
		}

		Node* SceneManager::GetCurrentSceneNode()
		{
			if (mCurrentScene)
				return mCurrentScene->GetNode();
			else
				return NULL;
		}

		void SceneManager::Initialize()
		{
			if (mCurrentScene)
			{
				mCurrentScene->Initialize();
			}
		}

		void SceneManager::Update(float deltaTime)
		{
			if (mCurrentScene)
			{
				mCurrentScene->Update(deltaTime);				
			}
		}
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
	}
}