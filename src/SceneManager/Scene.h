#pragma once
#include "Node.h"
//#include "Renderer/Batcher.h"
//#include "Renderer/Renderer.h"
#include "Renderer/Camera/Camera.h"
#include <Renderer/Camera/EditorCamera.h>
#include <Renderer/Camera/SceneCamera.h>
#include "Primitive/Skybox.h"

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
		Scene(std::string name);
		~Scene();
		
		void AddEditorCamera(Ref<EditorCamera> editorCamera);
		void AddSceneCamera(Ref<SceneCamera> sceneCamera);

		void Flush();

		//BatchButton m_BatchButton;
		//bool m_BatchingEnabled;
		//void OnBatched();
		//void OnUnBatched();

		void UpdateCameraRT(Ref<Camera> camera, Ref<Shader> shader, float deltaTime, bool isEditorCamera);
		void Render(Ref<Shader> shader, float deltaTime);

#ifdef TS_ENGINE_EDITOR
		int GetSkyboxEntityID();
#endif

		void SetCurrentSceneCamera(Ref<SceneCamera> sceneCamera);
		void SwitchToAnotherSceneCamera(Ref<SceneCamera> sceneCamera);
		void RemoveSceneCamera(Ref<SceneCamera> sceneCamera);

		Ref<Node> GetSceneNode() const { return mSceneNode; }
		Ref<SceneCamera> GetCurrentSceneCamera() { return mSceneCameras[mCurrentSceneCameraIndex]; }
		int GetCurrentSceneCameraIndex() { return mCurrentSceneCameraIndex; }
		std::vector<Ref<SceneCamera>> GetSceneCameras() { return mSceneCameras; }
		size_t GetNumSceneCameras() { return mSceneCameras.size(); }		

#ifdef TS_ENGINE_EDITOR
		void ShowSceneCameraGUI(Ref<Shader> shader, float deltaTime);
		Ref<EditorCamera> GetEditorCamera() { return mEditorCamera; }
#endif
	private:
		// Editor camera
#ifdef TS_ENGINE_EDITOR
		Ref<EditorCamera> mEditorCamera = nullptr;
#endif
		// Scene camera
		std::vector<Ref<SceneCamera>> mSceneCameras = {};
		int mCurrentSceneCameraIndex = 0;

		// Root node
		Ref<Node> mSceneNode;
		
		// Skybox
		Ref<TS_ENGINE::Skybox> mSkybox;
		//ButtonHandler mBatchButtonHandler;
	};
}