#include "tspch.h"
#include "SceneManager/Scene.h"

namespace TS_ENGINE
{
	Scene::Scene(std::string name):
		m_BatchingEnabled(false)
	{
		mSceneNode = CreateRef<Node>();
		mSceneNode->SetName(name);

		m_BatchButton.RegisterClickHandler(std::bind(&ButtonHandler::OnButtonClicked, &mBatchButtonHandler, std::placeholders::_1, std::placeholders::_2));
	}

	Scene::~Scene()
	{
		mSceneNode->Destroy();
		mSceneNode.reset();
	}

	void Scene::OnBatched()
	{
		mSceneNode->RemoveAllChildren();
		Ref<Node> batchedNode = Batcher::GetInstance()->GetBatchedNode();
		mSceneNode->AddChild(batchedNode);
		batchedNode->SetParentNode(mSceneNode);
	}

	void Scene::OnUnBatched()
	{
		mSceneNode->RemoveChild(Batcher::GetInstance()->GetBatchedNode());
		Batcher::GetInstance()->GetBatchedNode()->GetTransform()->Reset();
	}

	void Scene::Draw(Ref<Shader> shader)
	{
		if (mSceneNode)
		{
			mSceneNode->Draw(Matrix4(1), shader);
		}
		else
			TS_CORE_ERROR("Scene node not set");
	}
}
