#include "tspch.h"
#include "Camera.h"
#include "Core/Input.h"

namespace TS_ENGINE {

	Camera::Camera(const std::string& name) : 
		mDefaultPos(0, 0, 0),
		mDefaultEulerAngles(0, 0, 0),
		mMoveSpeed(0.02f),
		mRotateSpeed(0.02f)
	{
		mProjectionType = ProjectionType::PERSPECTIVE;
	}

	Camera::~Camera()
	{
		mDefaultPos = Vector3(0, 0, 0);
		mDefaultEulerAngles = Vector3(0, 0, 0);
		mMoveSpeed = 0.02f;
		mRotateSpeed = 0.02f;
		mProjectionType = ProjectionType::PERSPECTIVE;
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

	const Matrix4 Camera::GetProjectionViewMatrix() const 
	{ 
		return mProjectionMatrix * mViewMatrix; 
	}
}
