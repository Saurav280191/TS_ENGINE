#include <tspch.h>
#include "EditorCamera.h"
#include "Core/Input.h"
#include "Primitive/Sphere.h"
#include "SceneManager/Node.h"
#include "Renderer/MaterialManager.h"

namespace TS_ENGINE {

	EditorCamera::EditorCamera(const std::string& name) : 
		Camera(name)
	{
		mCameraNode = CreateRef<Node>();
		mCameraNode->SetNodeRef(mCameraNode);
		//mCameraNode->SetName(name);
		mCameraType = TS_ENGINE::Camera::Type::EDITORCAMERA;

		mDefaultPos = Vector3(8.56551170f, 3.30869126f, 9.40866184f);
		mDefaultEulerAngles = Vector3(0.493025780f, -0.706756115f, 0.0f);

		mCameraNode->GetTransform()->SetLocalPosition(mDefaultPos);
		mCameraNode->GetTransform()->SetLocalEulerAngles(mDefaultEulerAngles);
		mCameraNode->Initialize(name, EntityType::CAMERA);
	}

	EditorCamera::~EditorCamera()
	{

	}

	void EditorCamera::Initialize()
	{

	}

	void EditorCamera::Update(float deltaTime)
	{
		mViewMatrix = mCameraNode->GetTransform()->GetGlobalTransformationMatrix();;
		mViewMatrix = glm::inverse(mViewMatrix);

		for (auto& material : MaterialManager::GetInstance()->GetAllMaterials())
		{
			material->GetShader()->SetVec3("u_ViewPos", mCameraNode->GetTransform()->GetLocalPosition());

			if (mIsDistanceIndependent) // ProjectionMatrix is send to shader in both the cases of this if condition. 						
			{							// This difference from scene camera in this case. 
				material->GetShader()->SetMat4("u_View", Matrix4((Matrix3)mViewMatrix));
			}
			else
			{
				material->GetShader()->SetMat4("u_View", mViewMatrix);
			}

			material->GetShader()->SetMat4("u_Projection", mProjectionMatrix);
		}
	}

	void EditorCamera::DeleteMeshes()
	{

	}

	void EditorCamera::Controls(float deltaTime)
	{
		//Vertical
		if (Input::IsKeyPressed(Key::W))
		{
			mCameraNode->GetTransform()->MoveFwd(mMoveSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}
		if (Input::IsKeyPressed(Key::S))
		{
			mCameraNode->GetTransform()->MoveBack(mMoveSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}

		//Horizontal
		if (Input::IsKeyPressed(Key::D))
		{
			mCameraNode->GetTransform()->MoveRight(mMoveSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}
		if (Input::IsKeyPressed(Key::A))
		{
			mCameraNode->GetTransform()->MoveLeft(mMoveSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}

		//Move Up/Down
		if (Input::IsKeyPressed(Key::Space))
		{
			mCameraNode->GetTransform()->MoveUp(mMoveSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}
		if (Input::IsKeyPressed(Key::C))
		{
			mCameraNode->GetTransform()->MoveDown(mMoveSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}

		//Yaw - Keyboard
		if (Input::IsKeyPressed(Key::Right))
		{
			mCameraNode->GetTransform()->Yaw(mRotateSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}
		if (Input::IsKeyPressed(Key::Left))
		{
			mCameraNode->GetTransform()->Yaw(-mRotateSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}

		//Pitch - Keyboard
		if (Input::IsKeyPressed(Key::Up))
		{
			mCameraNode->GetTransform()->Pitch(mRotateSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}
		if (Input::IsKeyPressed(Key::Down))
		{
			mCameraNode->GetTransform()->Pitch(-mRotateSpeed, deltaTime);
			mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
		}

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
					mCameraNode->GetTransform()->Yaw(20.0f * deltaMousePos.x, deltaTime);
					mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
				}
				//Pitch Mouse
				if (deltaMousePos.y != 0)
				{
					mCameraNode->GetTransform()->Pitch(-20.0f * deltaMousePos.y, deltaTime);
					mCameraNode->GetTransform()->ComputeTransformationMatrix(mCameraNode->GetParentNode());
				}
			}
		}

	}
}

