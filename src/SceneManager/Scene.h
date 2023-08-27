#pragma once
#include "Node.h"
//#include "Renderer/Batcher.h"
//#include "Renderer/Renderer.h"
#include "Renderer/Camera/Camera.h"
#include <Renderer/Camera/EditorCamera.h>
#include <Renderer/Camera/SceneCamera.h>

#include <imgui.h>
//#define IMGUI_DEFINE_MATH_OPERATORS // Already set in preprocessors
#include "imgui_internal.h"
#define IMAPP_IMPL
#include "ImGuizmo.h"

namespace TS_ENGINE {

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
	class Camera;
	class SceneCamera;

	class Scene
	{
	public:
		Scene(std::string name, Ref<Camera> editorCamera);
		~Scene();

		//BatchButton m_BatchButton;
		//bool m_BatchingEnabled;
		//void OnBatched();
		//void OnUnBatched();

		void Render(Ref<Shader> shader, float deltaTime);
		Node* GetSceneNode() const { return mSceneNode; }
		Ref<SceneCamera> GetCurrentSceneCamera() { return mCurrentSceneCamera; }
		void UpdateCameraRT(Ref<Camera> camera, Ref<Shader> shader, float deltaTime, bool isEditorCamera);	

		int GetSkyboxEntityID() { return mSkyboxNode->GetEntity()->GetEntityID(); }
	private:
		Node* mSceneNode;		
		Ref<Camera> mEditorCamera = nullptr;
		std::vector<Ref<Camera>> mSceneCameras = {};
		Ref<SceneCamera> mCurrentSceneCamera;
		//ButtonHandler mBatchButtonHandler;
		Ref<TS_ENGINE::Node> mGroundNode;
		Ref<TS_ENGINE::Node> mCubeNode;
		Ref<TS_ENGINE::Node> mCube1Node;
		Ref<TS_ENGINE::Node> mSkyboxNode;
		Ref<TS_ENGINE::Node> mModelNode;
	};
}