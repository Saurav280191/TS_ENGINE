#include "tspch.h"
#include "SceneManager/Scene.h"

namespace TS_ENGINE
{
	Scene::Scene(std::string name)
	{
		mSceneNode = CreateRef<Node>();
		mSceneNode->SetName(name);
	}

	Scene::~Scene()
	{
		mSceneNode->Destroy();
		mSceneNode.reset();
	}

	void Scene::Draw(Ref<Shader> shader)
	{
		if (mSceneNode)
			mSceneNode->Draw(Matrix4(1), shader);
		else
			TS_CORE_ERROR("Scene node not set");
	}
}
