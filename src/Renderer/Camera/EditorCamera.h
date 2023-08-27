#pragma once
#include "Core/tspch.h"
#include "Renderer/Camera/Camera.h"
#include "Primitive/Sphere.h"
#include "SceneManager/Node.h"
#include "Core/Transform.h"

namespace TS_ENGINE {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera(const std::string& name);
		~EditorCamera();

		// Inherited via Camera	
		virtual void Initialize() override;
		virtual void Update(Ref<TS_ENGINE::Shader> shader, float deltaTime) override;
		virtual void DeleteMeshes() override;
		
		virtual Ref<Node> GetNode() override { return mCameraNode; }

		void Controls(float deltaTime);
	};
}
