#pragma once
#include "Core/tspch.h"
#include "Core/Object.h"
#include "Renderer/Shader.h"
#include "Camera.h"
#include "Renderer/Material.h"
#include "Primitive/Quad.h"
#include "Primitive/Line.h"
#include "SceneManager/SceneManager.h"
namespace TS_ENGINE
{
	class SceneCamera : public Camera
	{
	public:
#ifdef TS_ENGINE_EDITOR
		SceneCamera(const std::string& name, Ref<Camera> editorCamera);		
#else
		SceneCamera(const std::string& name);		
#endif
		// Inherited via Camera
		virtual void Initialize() override;
		virtual void SetName(const std::string& name) override;		
		virtual void Update(Ref<Shader> shader, float deltaTime) override;

		void CheckIfSelected(Ref<TS_ENGINE::Node>& hoveredOnNode);

		Ref<TS_ENGINE::Quad> GetSceneCameraGui()
		{
			return mSceneCameraGui;
		}
	private:
		Ref<TS_ENGINE::Shader> mSkyboxShader;
		Ref<TS_ENGINE::Material> mSkyboxMat;
#ifdef TS_ENGINE_EDITOR
		Ref<TS_ENGINE::Camera> mEditorCamera;
		Ref<TS_ENGINE::Texture2D> mCameraIcon;
		Ref<TS_ENGINE::Quad> mSceneCameraGui;
		Ref<TS_ENGINE::Line> mFrustrumLine;		
#endif
	};
}