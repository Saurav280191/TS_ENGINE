#pragma once
#include "Core/tspch.h"
#include "Renderer/Shader.h"
#include "Renderer/Framebuffer.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	class Camera
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

		enum Type
		{
			EDITORCAMERA,
			SCENECAMERA
		};
		enum ProjectionType
		{
			PERSPECTIVE,
			ORTHOGRAPHIC
		};

		Camera(const std::string& name);
		~Camera();
		void Flush();

		virtual void Initialize() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void DeleteMeshes() = 0;

		void CreateFramebuffer(uint32_t _width, uint32_t _height);

		void SetProjectionType(ProjectionType projectionType);
		//void SetOrthographic(float left, float right, float top, float bottom, float zNear, float zFar);
		//void SetOrthographic(Orthographic orthographic);
		void SetOrthographic(float size, float zNear, float zFar);
		void SetPerspective(float fov, float aspectRatio, float zNear, float zFar);
		void SetPerspective(Perspective perspective);
		void SetIsDistanceIndependent(bool distanceIndependent);
		void SetFieldOfView(float fov);
		void SetNearPlane(float zNear);
		void SetFarPlane(float zFar);
		void SetOrthographicSize(float size);

		//Getters
		const ProjectionType Camera::GetProjectionType() const { return mProjectionType; }
		const Orthographic& Camera::GetOrthographic() const { return mOrthographic; }
		const Perspective& Camera::GetPerspective()	const { return mPerspective; }
		const Matrix4 GetProjectionMatrix() const { return mProjectionMatrix; }
		const Matrix4 GetViewMatrix() const { return mViewMatrix; }
		const Matrix4 GetProjectionViewMatrix() const;		

		const Ref<Framebuffer> GetFramebuffer() const { return mFramebuffer; }
		virtual Ref<Node> GetNode() = 0;
	protected:
		bool mIsDistanceIndependent = false;
		Ref<Node> mCameraNode;

		glm::vec2 mInitialMousePosition;

		Vector3 mDefaultPos;
		Vector3 mDefaultEulerAngles;

		float mMoveSpeed;
		float mRotateSpeed;

		Type mCameraType;
		ProjectionType mProjectionType;
		Matrix4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

		Orthographic mOrthographic;
		Perspective mPerspective;
		Ref<Framebuffer> mFramebuffer;
	};
}