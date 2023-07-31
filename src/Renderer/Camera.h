#pragma once
#include "Core/tspch.h"
#include "Core/Object.h"
#include "Renderer/Shader.h"

#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#define IMAPP_IMPL
#include "ImGuizmo.h"
#include "Framebuffer.h"

namespace TS_ENGINE
{
	class Camera : public Object
	{
	public:
		struct Orthographic
		{
		public:
			float left = 0.0f;
			float right = 0.0f;
			float top = 0.0f;
			float bottom = 0.0f;
			float zNear = 0.0f;
			float zFar = 0.0f;

			Orthographic()
			{

			}

			Orthographic(float left, float right, float top, float bottom, float zNear, float zFar)
			{
				this->left = left;
				this->right = right;
				this->top = top;
				this->bottom = bottom;
				this->zNear = zNear;
				this->zFar = zFar;
			}
		};

		struct Perspective
		{
		public:
			float fov = 0.0f;
			float aspectRatio = 0.0f;
			float zNear = 0.0f;
			float zFar = 0.0f;

			Perspective()
			{

			}

			Perspective(float fov, float aspectRatio, float zNear, float zFar)
			{
				this->fov = fov;
				this->aspectRatio = aspectRatio;
				this->zNear = zNear;
				this->zFar = zFar;
			}
		};

		enum CameraType
		{
			EDITORCAMERA,
			SCENECAMERA
		};
		enum ProjectionType
		{
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

		Camera();
		Camera(const std::string& name);
		~Camera();
		
		void CreateFramebuffer(uint32_t _width, uint32_t _height);

		Camera::ProjectionType GetProjectionType();
		const Camera::Orthographic& GetOrthographic();
		const Camera::Perspective& GetPerspective();

		void SetOrthographic(float left, float right, float top, float bottom, float zNear, float zFar);

		void SetOrthographic(Orthographic orthographic);

		void SetPerspective(float fov, float aspectRatio, float zNear, float zFar);

		void SetPerspective(Perspective perspective);

		const Matrix4 GetProjectionMatrix() const;
		const Matrix4 GetViewMatrix() const;
		const Matrix4 GetProjectionViewMatrix() const;

		void SetCurrentShader(Ref<Shader>& currentShader);
		void Controls(float deltaTime);

		Ref<Framebuffer> GetFramebuffer();
	protected:
		Ref<Shader> mCurrentShader;
		glm::vec2 mInitialMousePosition;

		Vector3 mDefaultPos;
		Vector3 mDefaultEulerAngles;

		float mMoveSpeed;
		float mRotateSpeed;

		CameraType mCameraType;
		ProjectionType mProjectionType;
		Matrix4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

		Orthographic mOrthographic;
		Perspective mPerspective;

		void Reset();
	private:
		Ref<Framebuffer> mFramebuffer;
	};
}