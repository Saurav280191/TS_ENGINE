#pragma once
#include "Scene.h"
//#include "ShaderManager/ShaderProgram.h"

namespace TS_ENGINE
{
	namespace SceneManager
	{
		class Scene;

		class SceneManager
		{
		private:
			static SceneManager* m_pInstance;
			Scene* mCurrentScene;
			SceneManager();
			~SceneManager();
		public:
			static SceneManager* GetInstance();

			void SetCurrentScene(Scene* scene);
			
			Scene* GetCurrentScene();
			Node* GetCurrentSceneNode();

			void Initialize();
			void Update(float deltaTime);
			//void Render(ShaderManager::ShaderProgram* shaderProgram, const Matrix4& viewMatrix, const Matrix4& projectionMatrix);

			//bool HasSiblingWithSameName(Node* parentNode, const char* name);
		};
	}
}

