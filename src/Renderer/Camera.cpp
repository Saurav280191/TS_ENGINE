#include "tspch.h"
#include "Camera.h"
#include "Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace TS_ENGINE
{
	Camera::Camera(CameraType cameraType) :
		mMoveSpeed(0.02f),
		mRotateSpeed(0.0003f)
	{	
		mForward = Vector3(0, 0, 1);
		mUp = Vector3(0, 1, 0);
		mRight = Vector3(1, 0, 0);

		//mDefaultPos = Vector3(0.0f, 0.0f, 5.0f);
		//mDefaultEulerAngles = Vector3(0.0f, 0.0f, 0.0f);
		mDefaultPos = Vector3(8.56551170f, 3.30869126f, 9.40866184f);// 0.493025780f);
		mDefaultEulerAngles = Vector3(0.493025780f, -0.706756115f, 0.0f);

		mPos = mDefaultPos;
		mEulerAngles = mDefaultEulerAngles;
		mCameraType = cameraType;
		mProjectionType = ProjectionType::PERSPECTIVE;		
	}

	Camera::~Camera()
	{

	}

	Camera::ProjectionType Camera::GetProjectionType()
	{
		return mProjectionType;
	}
	const Camera::Orthographic& Camera::GetOrthographic()
	{
		return *mOrthographic;
	}
	const Camera::Perspective& Camera::GetPerspective()
	{
		return *mPerspective;
	}

	void Camera::MoveFwd(float deltaTime)
	{
		mPos += mForward * mMoveSpeed * deltaTime;
		//TS_CORE_INFO("Camera moved forward to {0}, {1}, {2}", mPos.x, mPos.y, mPos.z);
	}
	void Camera::MoveBack(float deltaTime)
	{
		mPos -= mForward * mMoveSpeed * deltaTime;
		//TS_CORE_INFO("Camera moved back to {0}, {1}, {2}", mPos.x, mPos.y, mPos.z);
	}
	void Camera::MoveLeft(float deltaTime)
	{
		mPos -= mRight * mMoveSpeed * deltaTime;
		//TS_CORE_INFO("Camera moved left to {0}, {1}, {2}", mPos.x, mPos.y, mPos.z);
	}
	void Camera::MoveRight(float deltaTime)
	{
		mPos += mRight * mMoveSpeed * deltaTime;
		//TS_CORE_INFO("Camera moved right to {0}, {1}, {2}", mPos.x, mPos.y, mPos.z);
	}
	void Camera::MoveUp(float deltaTime)
	{
		mPos += mUp * mMoveSpeed * deltaTime;
		//TS_CORE_INFO("Camera moved up to {0}, {1}, {2}", mPos.x, mPos.y, mPos.z);
	}
	void Camera::MoveDown(float deltaTime)
	{
		mPos -= mUp * mMoveSpeed * deltaTime;
		//TS_CORE_INFO("Camera moved down to {0}, {1}, {2}", mPos.x, mPos.y, mPos.z);
	}

	void Camera::Yaw(float yawSpeed, float deltaTime)
	{
		mEulerAngles.y -= mRotateSpeed * yawSpeed * deltaTime;
		//TS_CORE_INFO("Camera rotated to {0}, {1}, {2}", mEulerAngles.x, mEulerAngles.y, mEulerAngles.z);
	}
	void Camera::Pitch(float pitchSpeed, float deltaTime)
	{
		mEulerAngles.x -= mRotateSpeed * pitchSpeed * deltaTime;
		//TS_CORE_INFO("Camera rotated to {0}, {1}, {2}", mEulerAngles.x, mEulerAngles.y, mEulerAngles.z);
	}
	void Camera::Roll(float rollSpeed, float deltaTime)
	{
		mEulerAngles.z -= mRotateSpeed * rollSpeed * deltaTime;
		//TS_CORE_INFO("Camera rotated to {0}, {1}, {2}", mEulerAngles.x, mEulerAngles.y, mEulerAngles.z);
	}

	void Camera::Reset()
	{
		mPos = mDefaultPos;
		mEulerAngles = mDefaultEulerAngles;
		//TS_CORE_INFO("Camera resets to Pos: {0}, {1}, {2}, EulerAngles: {3}, {4}, {5}", mPos.x, mPos.y, mPos.z, mEulerAngles.x, mEulerAngles.y, mEulerAngles.z);
	}

	void Camera::SetPosition(Vector3& v)
	{
		mPos.x = v.x;
		mPos.y = v.y;
		mPos.z = v.z;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		mPos.x = x;
		mPos.y = y;
		mPos.z = z;
	}

	void Camera::SetEulerAngles(Vector3& v)
	{
		mEulerAngles.x = v.x;
		mEulerAngles.y = v.y;
		mEulerAngles.z = v.z;
	}

	void Camera::SetEulerAngles(float x, float y, float z)
	{
		mEulerAngles.x = x;
		mEulerAngles.y = y;
		mEulerAngles.z = z;
	}

	const Vector3& Camera::GetPosition() const
	{
		return mPos;
	}

	const Vector3& Camera::GetEulerAngles() const
	{
		return mEulerAngles;
	}

	const Matrix4 Camera::GetProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	const Matrix4 Camera::GetViewMatrix()
	{
		return mViewMatrix;
	}
	
	glm::quat Camera::GetOrientation()
	{
		return glm::quat(Vector3(-mEulerAngles.x, -mEulerAngles.y, -mEulerAngles.z));
	}

	void Camera::OnUpdate(float deltaTime)
	{
		glm::quat orientation = GetOrientation();
		mViewMatrix = glm::translate(glm::mat4(1.0f), mPos) * glm::toMat4(orientation);
		mViewMatrix = glm::inverse(mViewMatrix);

		mRight = Vector3(mViewMatrix[0][0], mViewMatrix[1][0], mViewMatrix[2][0]);
		mRight = glm::normalize(mRight);
		mUp = Vector3(mViewMatrix[0][1], mViewMatrix[1][1], mViewMatrix[2][1]);
		mUp = glm::normalize(mUp);
		mForward = -1.0f * Vector3(mViewMatrix[0][2], mViewMatrix[1][2], mViewMatrix[2][2]);
		mForward = glm::normalize(mForward);

		//Vertical
		if (Input::IsKeyPressed(Key::W))
			MoveFwd(deltaTime);
		if (Input::IsKeyPressed(Key::S))
			MoveBack(deltaTime);

		//Horizontal
		if (Input::IsKeyPressed(Key::D))
			MoveRight(deltaTime);
		if (Input::IsKeyPressed(Key::A))
			MoveLeft(deltaTime);

		//Move Up/Down
		if (Input::IsKeyPressed(Key::Space))
			MoveUp(deltaTime);
		if (Input::IsKeyPressed(Key::LeftControl))
			MoveDown(deltaTime);

		//Yaw - Keyboard
		if (Input::IsKeyPressed(Key::Right))
			Yaw(-1.0f, deltaTime);
		if (Input::IsKeyPressed(Key::Left))
			Yaw(1.0f, deltaTime);

		//Pitch - Keyboard
		if (Input::IsKeyPressed(Key::Up))
			Pitch(-1.0f, deltaTime);
		if (Input::IsKeyPressed(Key::Down))
			Pitch(1.0f, deltaTime);

		//Mouse Move
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 deltaMousePos = (mouse - mInitialMousePosition) * 0.003f;
			mInitialMousePosition = mouse;

			//Yaw - Mouse
			if (Input::IsMouseButtonPressed(Mouse::Button1))
			{
				if (deltaMousePos.x != 0)
				{
					if (deltaMousePos.x < 0)
						Yaw(1.0f, deltaTime);
					if (deltaMousePos.x > 0)
						Yaw(-1.0f, deltaTime);
				}

				//Pitch Mouse
				if (deltaMousePos.y != 0)
				{
					if (deltaMousePos.y < 0)
						Pitch(1.0f, deltaTime);
					if (deltaMousePos.y > 0)
						Pitch(-1.0f, deltaTime);
				}
			}
		}
	}

	void Camera::SetOrthographic(Orthographic orthographic)
	{
		mProjectionType = ProjectionType::ORTHOGRAPHIC;
		mOrthographic = &orthographic;
		mProjectionMatrix = glm::ortho(orthographic.left, orthographic.right, orthographic.top,
			orthographic.bottom, orthographic.zNear, orthographic.zFar);
	}
	void Camera::SetOrthographic(float left, float right, float top, float bottom, float zNear, float zFar)
	{
		mProjectionType = ProjectionType::ORTHOGRAPHIC;
		mOrthographic = new Orthographic(left, right, top, bottom, zNear, zFar);
		mProjectionMatrix = glm::ortho(left, right, top, bottom, zNear, zFar);
	}
	
	void Camera::SetPerspective(Perspective perspective)
	{
		mProjectionType = ProjectionType::PERSPECTIVE;
		mPerspective = &perspective;
		mProjectionMatrix = glm::perspective(perspective.fov, perspective.aspectRatio, perspective.zNear, perspective.zFar);
	}
	void Camera::SetPerspective(float fov, float aspectRatio, float zNear, float zFar)
	{
		mProjectionType = ProjectionType::PERSPECTIVE;
		mPerspective = new Perspective(fov, aspectRatio, zNear, zFar);
		mProjectionMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
	}

}
