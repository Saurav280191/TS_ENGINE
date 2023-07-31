#pragma once
#include "Core/tspch.h"
#include "Core/Object.h"
#include "Renderer/Shader.h"
#include "Camera.h"
#include "Renderer/Material.h"

namespace TS_ENGINE
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera(const std::string& name);		
		// Inherited via Camera
		virtual void Initialize() override;
		virtual void SetName(const std::string& name) override;
		virtual void Update(float deltaTime) override;
	private:
		Ref<TS_ENGINE::Shader> mSkyboxShader;
		Ref<TS_ENGINE::Material> mSkyboxMat;

	};
}