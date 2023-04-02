#pragma once
#include "Core/tspch.h"

namespace TS_ENGINE
{
	class Camera
	{
	public:
		struct Orthographic
		{
		public:
			float left = 0.0f;
			float right= 0.0f;
			float top= 0.0f;
			float bottom= 0.0f;
			float zNear= 0.0f;
			float zFar= 0.0f;

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
			float fov= 0.0f;
			float aspectRatio= 0.0f;
			float zNear= 0.0f;
			float zFar= 0.0f;
			
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

	public:
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
	private:
		Vector3 mPos;
		Vector3 mEulerAngles;
		Vector3 mDefaultPos;
		Vector3 mDefaultEulerAngles;
		float mMoveSpeed;
		float mRotateSpeed;
		
		Vector3 mUp;
		Vector3 mRight;
		Vector3 mForward;

		glm::vec2 mInitialMousePosition;

		CameraType mCameraType;
		ProjectionType mProjectionType;
		Matrix4 mProjectionMatrix;
		glm::mat4 mViewMatrix;

		Orthographic* mOrthographic;
		Perspective* mPerspective;
	public:
		Camera(CameraType cameraType);
		~Camera();		

		Camera::ProjectionType GetProjectionType();
		const Camera::Orthographic& GetOrthographic();
		const Camera::Perspective& GetPerspective();

		void MoveFwd(float deltaTime);
		void MoveBack(float deltaTime);
		void MoveLeft(float deltaTime);
		void MoveRight(float deltaTime);
		void MoveUp(float deltaTime);
		void MoveDown(float deltaTime);

		void Yaw(float yawSpeed, float deltaTime);
		void Pitch(float pitchSpeed, float deltaTime);
		void Roll(float rollSpeed, float deltaTime);

		void Reset();

		void SetPosition(Vector3& v);
		void SetPosition(float x, float y, float z);
		void SetEulerAngles(Vector3& v);
		void SetEulerAngles(float x, float y, float z);

		const Vector3& GetPosition() const;
		const Vector3& GetEulerAngles() const;

		//void SetTransform(Transform* transform);

		void SetOrthographic(float left, float right, float top, float bottom, float zNear, float zFar);

		void SetOrthographic(Orthographic orthographic);

		void SetPerspective(float fov, float aspectRatio, float zNear, float zFar);

		void SetPerspective(Perspective perspective);

		const Matrix4 GetProjectionMatrix() const;		
		const Matrix4 GetViewMatrix();

		glm::quat GetOrientation();

		void OnUpdate(float deltaTime);
	};
}