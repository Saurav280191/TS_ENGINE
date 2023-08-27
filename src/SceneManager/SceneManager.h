#pragma once
#include "Scene.h"
//#include "SceneSerializer.h"

namespace TS_ENGINE
{
	class Scene;
	
	class SceneManager
	{
	public:
		static SceneManager* GetInstance();
		void SetCurrentScene(Ref<Scene> scene);
		//void Initialize();
		//void Update(float deltaTime);	
		//void Render(ShaderManager::ShaderProgram* shaderProgram, const Matrix4& viewMatrix, const Matrix4& projectionMatrix);
		//bool HasSiblingWithSameName(Node* parentNode, const char* name);
		void SaveCurrentScene();
		Ref<Scene> GetCurrentScene();
	private:
		static SceneManager* mInstance;
		Ref<Scene> mCurrentScene;
		SceneManager();
		~SceneManager();

		//Ref<SceneSerializer> mSceneSerializer;
		Ref<Node> GetCurrentSceneNode();
	};
}

