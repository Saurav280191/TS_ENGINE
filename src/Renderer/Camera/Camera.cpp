#include "tspch.h"
#include "Camera.h"
#include "Core/Input.h"

namespace TS_ENGINE {

	Camera::Camera(const std::string& name) : 
		mDefaultPos(0, 0, 0),
		mDefaultEulerAngles(0, 0, 0),
		mMoveSpeed(0.005f),
		mRotateSpeed(0.02f),
		mCameraType(Camera::Type::EDITORCAMERA),
		mProjectionType(ProjectionType::PERSPECTIVE)
	{

	}

	Camera::~Camera()
	{
		mDefaultPos = Vector3(0, 0, 0);
		mDefaultEulerAngles = Vector3(0, 0, 0);
		mMoveSpeed = 0.02f;
		mRotateSpeed = 0.02f;
		mProjectionType = ProjectionType::PERSPECTIVE;
	}

	void Camera::Flush()
	{
		mFramebuffer.reset();
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

	void Camera::SetProjectionType(ProjectionType projectionType)
	{
		mProjectionType = projectionType;

		if (mProjectionType == ProjectionType::PERSPECTIVE)
		{
			SetPerspective(60.0f, 1.77f, 0.5f, 50.0f);
		}
		else if (mProjectionType == ProjectionType::ORTHOGRAPHIC)
		{
			SetOrthographic(5.0f, 0.5f, 50.0f);
		}
	}

	/*void Camera::SetOrthographic(Orthographic orthographic)
	{
		mProjectionType = ProjectionType::ORTHOGRAPHIC;
		mOrthographic = orthographic;
		mProjectionMatrix = glm::ortho(orthographic.left, orthographic.right, orthographic.top,
			orthographic.bottom, orthographic.zNear, orthographic.zFar);
	}*/
	
	/*void Camera::SetOrthographic(float left, float right, float top, float bottom, float zNear, float zFar)
	{
		mProjectionType = ProjectionType::ORTHOGRAPHIC;
		mOrthographic = Orthographic(left, right, top, bottom, zNear, zFar);
		mProjectionMatrix = glm::ortho(left, right, top, bottom, zNear, zFar);
	}*/
	
	void Camera::SetOrthographic(float size, float zNear, float zFar)
	{
		mProjectionType = ProjectionType::ORTHOGRAPHIC;
		mOrthographic.top = size;
		mOrthographic.bottom = -size;
		mOrthographic.left = -size / 0.5625f;// 9/16 = 0.5625f
		mOrthographic.right = size / 0.5625f;
		mOrthographic.zNear = zNear;
		mOrthographic.zFar = zFar;

		mProjectionMatrix = glm::ortho(mOrthographic.left, mOrthographic.right, mOrthographic.bottom, mOrthographic.top, mOrthographic.zNear, mOrthographic.zFar);
	}

	void Camera::SetPerspective(Perspective perspective)
	{
		mProjectionType = ProjectionType::PERSPECTIVE;
		mPerspective = perspective;
		mProjectionMatrix = glm::perspective(glm::radians(perspective.fov), perspective.aspectRatio, perspective.zNear, perspective.zFar);
	}
	void Camera::SetPerspective(float fov, float aspectRatio, float zNear, float zFar)
	{
		mProjectionType = ProjectionType::PERSPECTIVE;
		mPerspective = Perspective(fov, aspectRatio, zNear, zFar);
		mProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
	}
	
	void Camera::SetIsDistanceIndependent(bool distanceIndependent)
	{
		mIsDistanceIndependent = distanceIndependent;
	}

	void Camera::SetFieldOfView(float fov)
	{
		mPerspective.fov = fov;
		mProjectionMatrix = glm::perspective(glm::radians(mPerspective.fov), mPerspective.aspectRatio, mPerspective.zNear, mPerspective.zFar);
	}

	void Camera::SetNearPlane(float zNear)
	{
		mPerspective.zNear = zNear;
		mOrthographic.zNear = zNear;

		if (mProjectionType == ProjectionType::PERSPECTIVE)
			mProjectionMatrix = glm::perspective(glm::radians(mPerspective.fov), mPerspective.aspectRatio, mPerspective.zNear, mPerspective.zFar);
		else if (mProjectionType == ProjectionType::ORTHOGRAPHIC)
			mProjectionMatrix = glm::ortho(mOrthographic.left, mOrthographic.right, mOrthographic.top, mOrthographic.bottom, mOrthographic.zNear, mOrthographic.zFar);
	}

	void Camera::SetFarPlane(float zFar)
	{
		mPerspective.zFar = zFar;
		mOrthographic.zFar = zFar;

		if(mProjectionType == ProjectionType::PERSPECTIVE)
			mProjectionMatrix = glm::perspective(glm::radians(mPerspective.fov), mPerspective.aspectRatio, mPerspective.zNear, mPerspective.zFar);
		else if(mProjectionType == ProjectionType::ORTHOGRAPHIC)
			mProjectionMatrix = glm::ortho(mOrthographic.left, mOrthographic.right, mOrthographic.top, mOrthographic.bottom, mOrthographic.zNear, mOrthographic.zFar);
	}

	void Camera::SetOrthographicSize(float size)
	{
		mOrthographic.top = size;
		mOrthographic.bottom = -size;
		mOrthographic.left = -size / 0.5625f;// 9/16 = 0.5625f
		mOrthographic.right = size / 0.5625f;
		mProjectionMatrix = glm::ortho(mOrthographic.left, mOrthographic.right, mOrthographic.bottom, mOrthographic.top, mOrthographic.zNear, mOrthographic.zFar);
	}

	const Matrix4 Camera::GetProjectionViewMatrix() const 
	{ 
		return mProjectionMatrix * mViewMatrix; 
	}	
}
