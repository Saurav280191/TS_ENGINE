#pragma once
#include "Node.h"
//#include "Renderer/Batcher.h"
//#include "Renderer/Renderer.h"
#include "Renderer/Camera/Camera.h"
#include <Renderer/Camera/EditorCamera.h>
#include <Renderer/Camera/SceneCamera.h>
#include "Primitive/Skybox.h"
#include "Components/Animation.h"

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
		Scene();
		~Scene();
		
		void SetName(const std::string& _name);
		void Initialize();
		void AddEditorCamera(Ref<EditorCamera> editorCamera);
		void AddSceneCamera(Ref<SceneCamera> sceneCamera);
		
		void AttachSkybox(Ref<Skybox> _skybox);

		//BatchButton m_BatchButton;
		//bool m_BatchingEnabled;
		//void OnBatched();
		//void OnUnBatched();

		void Update(float _deltaTime);

		// 1. Binds camera's framebuffer
		// 2. Clears color
		// 3. Clear entity ID attachment to -1
		// 4. Renders skybox
		// 5. Renders scene hierarchy
		// 6. Unbinds camera's framebuffer
		void Render(Ref<Shader> shader, float deltaTime);
		
		void UpdateCameraRT(Ref<Camera> camera, Ref<Shader> shader, float deltaTime, bool isEditorCamera);
#ifdef TS_ENGINE_EDITOR
		int GetSkyboxNodeId();
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

		void AddAnimation(Ref<Animation> _animation);
		
		void Destroy();
	public:
		std::string mName;										// Name
	private:
		// Editor camera
#ifdef TS_ENGINE_EDITOR
		Ref<EditorCamera> mEditorCamera = nullptr;				// Editor Camera
#endif
		// Scene cameras
		std::vector<Ref<SceneCamera>> mSceneCameras = {};		// Scene Cameras
		int mCurrentSceneCameraIndex = 0;						

		// Skybox
		Ref<TS_ENGINE::Skybox> mSkybox;							// Skybox
		//ButtonHandler mBatchButtonHandler;

		// Root node
		Ref<Node> mSceneNode;									// Scene root node

		std::unordered_map<std::string,							// Animations 
			Ref<Animation>> mAnimations;
	
	public:
		int mSelectedBoneId;									// SelectedBoneId
		int mSelectedModelRootNodeId;							// SelectedModelRootNodeId
	};
}