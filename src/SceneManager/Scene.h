#pragma once
#include "Node.h"
//#include "Renderer/Batcher.h"
#include "Renderer/Renderer.h"

namespace TS_ENGINE
{
#pragma region Batching classes

	class BatchButton
	{
	public:
		typedef std::function<void(Ref<TS_ENGINE::Shader>& currentShader, std::vector<Ref<TS_ENGINE::Node>> nodes)> ClickHandler;

		void RegisterClickHandler(ClickHandler handler)
		{
			mClickHandler = handler;
		}

		void Click(Ref<TS_ENGINE::Shader>& currentShader, std::vector<Ref<TS_ENGINE::Node>> nodes)
		{
			mClickHandler(currentShader, nodes);
		}

	private:
		ClickHandler mClickHandler;
	};

	//class ButtonHandler
	//{
	//public:
	//	void OnButtonClicked(const Ref<TS_ENGINE::Shader>& currentShader, std::vector<Ref<TS_ENGINE::Node>> nodes)
	//	{
	//		currentShader->Bind();

	//		if (!Batcher::GetInstance()->IsBatched())
	//		{
	//			if (nodes.size() == 0)
	//			{
	//				TS_CORE_WARN("No gameObjects to batch!");
	//				return;
	//			}

	//			TS_CORE_INFO("***Started batching gameobjects***");

	//			//Reset stats
	//			Application::Get().ResetStats();

	//			if (nodes.size() > 0)
	//			{
	//				Batcher::GetInstance()->CreateAtlases();
	//				Batcher::GetInstance()->StartBatching(nodes);
	//			}
	//		}
	//	}
	//};

#pragma endregion

	class Scene
	{
	public:
		Scene(std::string name);		
		~Scene();
	
		//BatchButton m_BatchButton;
		//bool m_BatchingEnabled;
		
		//void OnBatched();
		//void OnUnBatched();

		const Ref<Node> GetSceneNode() const
		{
			return mSceneNode;
		}
		
		void Initialize(Ref<Camera> editorCamera);
		void Update(Ref<Shader> shader, float deltaTime);
		Ref<Node> PickNodeByEntityID(int entityID);		
	private:		
		Ref<Node> mSceneNode;
		void PickNode(Ref<Node> node, int entityID);		

		Ref<Node> mMatchingNode = nullptr;
		Ref<Camera> mEditorCamera = nullptr;
		std::vector<Ref<Camera>> mSceneCameras = {};

		//ButtonHandler mBatchButtonHandler;
	};
}


