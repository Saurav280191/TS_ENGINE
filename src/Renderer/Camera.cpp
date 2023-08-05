#include "tspch.h"
#include "Camera.h"
#include "Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <Core/Application.h>

namespace TS_ENGINE
{
	Camera::Camera()
	{

	}

	Camera::Camera(const std::string& name) :
		mMoveSpeed(0.02f),
		mRotateSpeed(0.02f)
	{	
		mName = name;
		mEntityType = EntityType::CAMERA;

		mDefaultPos = Vector3(8.56551170f, 3.30869126f, 9.40866184f);
		mDefaultEulerAngles = Vector3(0.493025780f, -0.706756115f, 0.0f);

		mNode->GetTransform()->SetLocalPosition(mDefaultPos);
		mNode->GetTransform()->SetLocalEulerAngles(mDefaultEulerAngles);
		mProjectionType = ProjectionType::PERSPECTIVE;
	}


	Camera::~Camera()
	{

	}

	void Camera::CreateFramebuffer(uint32_t _width, uint32_t _height)
	{
		//Setup framebuffer
		TS_ENGINE::FramebufferSpecification fbSpec;
		fbSpec.Attachments = { TS_ENGINE::FramebufferTextureFormat::RGBA8, TS_ENGINE::FramebufferTextureFormat::RED_INTEGER, TS_ENGINE::FramebufferTextureFormat::Depth };
		fbSpec.Width = _width;
		fbSpec.Height = _height;
		mFramebuffer = TS_ENGINE::Framebuffer::Create(fbSpec);
	}

	Camera::ProjectionType Camera::GetProjectionType()
	{
		return mProjectionType;
	}
	const Camera::Orthographic& Camera::GetOrthographic()
	{
		return mOrthographic;
	}
	const Camera::Perspective& Camera::GetPerspective()
	{
		return mPerspective;
	}

	const Matrix4 Camera::GetProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	const Matrix4 Camera::GetViewMatrix() const
	{
		return mViewMatrix;
	}

	const Matrix4 Camera::GetProjectionViewMatrix() const
	{
		return mProjectionMatrix * mViewMatrix;
	}

	//void Camera::SetCurrentShader(Ref<Shader>& currentShader)
	//{
		//mCurrentShader = currentShader;
	//}

	void Camera::SetOrthographic(Orthographic orthographic)
	{
		mProjectionType = ProjectionType::ORTHOGRAPHIC;
		mOrthographic = orthographic;
		mProjectionMatrix = glm::ortho(orthographic.left, orthographic.right, orthographic.top,
			orthographic.bottom, orthographic.zNear, orthographic.zFar);
	}
	void Camera::SetOrthographic(float left, float right, float top, float bottom, float zNear, float zFar)
	{
		mProjectionType = ProjectionType::ORTHOGRAPHIC;
		mOrthographic = Orthographic(left, right, top, bottom, zNear, zFar);
		mProjectionMatrix = glm::ortho(left, right, top, bottom, zNear, zFar);
	}
	
	void Camera::SetPerspective(Perspective perspective)
	{
		mProjectionType = ProjectionType::PERSPECTIVE;
		mPerspective = perspective;
		mProjectionMatrix = glm::perspective(perspective.fov, perspective.aspectRatio, perspective.zNear, perspective.zFar);
	}
	void Camera::SetPerspective(float fov, float aspectRatio, float zNear, float zFar)
	{
		mProjectionType = ProjectionType::PERSPECTIVE;
		mPerspective = Perspective(fov, aspectRatio, zNear, zFar);
		mProjectionMatrix = glm::perspective(fov, aspectRatio, zNear, zFar);
	}

	void Camera::Controls(float deltaTime)
	{
		//Vertical
		if (Input::IsKeyPressed(Key::W))
			mNode->GetTransform()->MoveFwd(mMoveSpeed, deltaTime);
		if (Input::IsKeyPressed(Key::S))
			mNode->GetTransform()->MoveBack(mMoveSpeed, deltaTime);

		//Horizontal
		if (Input::IsKeyPressed(Key::D))
			mNode->GetTransform()->MoveRight(mMoveSpeed, deltaTime);
		if (Input::IsKeyPressed(Key::A))
			mNode->GetTransform()->MoveLeft(mMoveSpeed, deltaTime);

		//Move Up/Down
		if (Input::IsKeyPressed(Key::Space))
			mNode->GetTransform()->MoveUp(mMoveSpeed, deltaTime);
		if (Input::IsKeyPressed(Key::LeftControl))
			mNode->GetTransform()->MoveDown(mMoveSpeed, deltaTime);

		//Yaw - Keyboard
		if (Input::IsKeyPressed(Key::Right))
			mNode->GetTransform()->Yaw(mRotateSpeed, deltaTime);
		if (Input::IsKeyPressed(Key::Left))
			mNode->GetTransform()->Yaw(-mRotateSpeed, deltaTime);
			
		//Pitch - Keyboard
		if (Input::IsKeyPressed(Key::Up))
			mNode->GetTransform()->Pitch(mRotateSpeed, deltaTime);
		if (Input::IsKeyPressed(Key::Down))
			mNode->GetTransform()->Pitch(-mRotateSpeed, deltaTime);

		//Mouse Move
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 deltaMousePos = (mouse - mInitialMousePosition) * 0.003f;
			mInitialMousePosition = mouse;

			//Yaw - Mouse
			if (Input::IsMouseButtonPressed(Mouse::Button1))
			{
				if (deltaMousePos.x != 0)
					mNode->GetTransform()->Yaw(20.0f * deltaMousePos.x, deltaTime);

				//Pitch Mouse
				if (deltaMousePos.y != 0)
					mNode->GetTransform()->Pitch(-20.0f * deltaMousePos.y, deltaTime);
			}
		}
	}

	void Camera::Reset()
	{
		mNode->GetTransform()->SetLocalPosition(mDefaultPos);
		mNode->GetTransform()->SetLocalEulerAngles(mDefaultEulerAngles);
	}

	Ref<Framebuffer> Camera::GetFramebuffer()
	{
		return mFramebuffer;
	}
}
