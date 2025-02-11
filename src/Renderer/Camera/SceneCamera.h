#pragma once
#include "Core/tspch.h"
#include "Renderer/Shader.h"
#include "Camera.h"
#include "Renderer/Material.h"
#include "Primitive/Quad.h"
#include "Primitive/Line.h"
#include "SceneManager/SceneManager.h"

namespace TS_ENGINE {

	class SceneCamera : public Camera
	{
	public:
#ifdef TS_ENGINE_EDITOR
		SceneCamera(const std::string& name, Ref<Camera> editorCamera);
#else
		SceneCamera(const std::string& name);
#endif
		~SceneCamera();

		// Inherited via Camera
		virtual void Initialize() override;
		virtual void Update(Ref<Shader> shader, float deltaTime) override;
		virtual void DeleteMeshes() override;

		void ShowCameraGUI(Ref<Shader> shader, float deltaTime);

#ifdef TS_ENGINE_EDITOR
		void ShowFrustrumGUI(Ref<Shader> shader, float deltaTime);
		bool IsSceneCameraGuiSelected(int _nodeId);
		void RefreshFrustrumGUI();
		Ref<Node> GetSceneCameraGui() { return mSceneCameraGuiNode; }
#else
		void Controls(float deltaTime);
#endif
		virtual Ref<Node> GetNode() override { return mCameraNode; }
		
	private:
#ifdef TS_ENGINE_EDITOR
		Ref<Camera> mEditorCamera;
		Ref<Texture2D> mCameraIcon;
		
		//GUI
		Ref<Node> mSceneCameraFrustrumNode;
		Ref<Node> mSceneCameraGuiNode;
#endif
	};
}