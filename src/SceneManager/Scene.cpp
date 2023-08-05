#include "tspch.h"
#include "SceneManager/Scene.h"
#include <Renderer/Camera.h>

namespace TS_ENGINE
{
	Scene::Scene(std::string name)//:
		//m_BatchingEnabled(false)
	{
		mSceneNode = CreateRef<Node>();
		mSceneNode->SetName(name);

		//m_BatchButton.RegisterClickHandler(std::bind(&ButtonHandler::OnButtonClicked, &mBatchButtonHandler, std::placeholders::_1, std::placeholders::_2));
	}

	Scene::~Scene()
	{
		mSceneNode->Destroy();
		mSceneNode.reset();
	}

	/*void Scene::OnBatched()
	{
		mSceneNode->RemoveAllChildren();
		Ref<Node> batchedNode = Batcher::GetInstance()->GetBatchedNode();
		mSceneNode->AddChild(mSceneNode, batchedNode);
		batchedNode->SetParentNode(mSceneNode);
	}*/

	/*void Scene::OnUnBatched()
	{
		mSceneNode->RemoveChild(Batcher::GetInstance()->GetBatchedNode());
		Batcher::GetInstance()->GetBatchedNode()->GetTransform()->Reset();
	}*/

	void Scene::Initialize(Ref<Camera> editorCamera)
	{
		mEditorCamera = editorCamera;
		Renderer::BeginScene(editorCamera);
		mSceneNode->InitializeTransformMatrices();
	}

	void Scene::Update(Ref<Shader> shader, float deltaTime)
	{
		if (mSceneNode)		
			mSceneNode->Update(shader, deltaTime);		
		else
			TS_CORE_ERROR("Scene node not set");
	}

	Ref<Node> Scene::PickNodeByEntityID(int entityID)
	{		
		PickNode(mSceneNode, entityID);
		return mMatchingNode;
	}

	void Scene::PickNode(Ref<Node> node, int entityID)
	{
		if (node->HasAttachedObject())
		{
			TS_CORE_TRACE("Checking: {0}", node->GetName());

			if (node->GetAttachedObject()->GetEntityID() == entityID)
			{
				TS_CORE_TRACE("{0} has matching entityID", node->GetName());
				mMatchingNode = node;
			}
		}

		for (auto& childNode : node->GetChildren())
		{
			PickNode(childNode, entityID);
		}
	}
}
