#pragma once
#include "Scene.h"

#ifdef TS_ENGINE_EDITOR
#include "SceneSerializer.h"
#endif

namespace TS_ENGINE
{
	class Scene;
	
	class SceneManager
	{
	public:
		static SceneManager* GetInstance();
		void SetCurrentScene(Ref<Scene> scene);

		// Create a new scene with an editor camera, a scene camera and a ground 
		void CreateNewScene(const std::string& sceneName);
		void SaveCurrentScene();
		void LoadScene(const std::string& savedScenePath);
		Ref<Scene> GetCurrentScene();

		void FlushCurrentScene();
	private:
		static SceneManager* mInstance;
		Ref<Scene> mCurrentScene;
		SceneManager();
		~SceneManager();

#ifdef TS_ENGINE_EDITOR
		Ref<SceneSerializer> mSceneSerializer;
#endif

		Ref<Node> GetCurrentSceneNode();
	};
}

