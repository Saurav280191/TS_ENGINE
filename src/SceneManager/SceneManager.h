#pragma once
#include "Scene.h"
#include "SceneSerializer.h"

namespace TS_ENGINE
{
	class Scene;
	
	class SceneManager
	{
	public:
		static SceneManager* GetInstance();
		void SetCurrentScene(Ref<Scene> scene);

		void SaveCurrentScene();
		Ref<Scene> GetCurrentScene();
	private:
		static SceneManager* mInstance;
		Ref<Scene> mCurrentScene;
		SceneManager();
		~SceneManager();

		Ref<SceneSerializer> mSceneSerializer;
		Ref<Node> GetCurrentSceneNode();
	};
}

