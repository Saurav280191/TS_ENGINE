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
		void SetCurrentScene(const Ref<Scene>& scene);

		// Create a new scene with an editor camera, a scene camera and a ground 
		void CreateNewScene(const std::string& sceneName);
		void SaveCurrentScene();
		void LoadScene(const std::string& savedScenePath);
		Ref<Scene>& GetCurrentScene();

		void ClearCurrentScene();
		void ClearNodeMaps();

		// Adds node to map and returns the id from map
		uint32_t Register(Ref<Node> _node);
		// Removes the node from mNodeMap and adds the removed Id in mFreeIds queue
		void Deregister(uint32_t _id);

		Ref<Node> GetNode(int _id)
		{
			TS_CORE_ASSERT(mNodeMap[_id]);
			return mNodeMap[_id];
		}
	private:
		SceneManager();
		~SceneManager();

		Ref<Node> GetCurrentSceneNode();

		static SceneManager* mInstance;
		Ref<Scene> mCurrentScene;

#ifdef TS_ENGINE_EDITOR
		Ref<SceneSerializer> mSceneSerializer;
#endif				

		std::unordered_map<uint32_t, Ref<Node>> mNodeMap = {};	// Unique Id and Node Map
		std::queue<uint32_t> mFreeIds;							// ID recycling queue
		uint32_t mNextId = 1;									// Unique Id Counter
	};
}

