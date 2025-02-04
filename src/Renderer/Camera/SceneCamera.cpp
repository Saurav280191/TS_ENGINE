#include "tspch.h"
#include "SceneCamera.h"
#include "Input.h"
#include <GLM/gtx/quaternion.hpp>
#include "Core/Factory.h"

namespace TS_ENGINE {

#ifdef TS_ENGINE_EDITOR
	SceneCamera::SceneCamera(const std::string& name, Ref<Camera> editorCamera) : 
		Camera(name)
	{
		mCameraNode = CreateRef<Node>();
		mCameraNode->SetNodeRef(mCameraNode);
		mCameraType = Type::SCENECAMERA;
		mEditorCamera = editorCamera;
	}
#else
	SceneCamera::SceneCamera(const std::string& name) : 
		Camera(name)
	{
		mCameraNode = CreateRef<Node>();
		mCameraNode->SetNodeRef(mCameraNode);
		mCameraType = Type::SCENECAMERA;
	}
#endif

	void SceneCamera::Initialize()
	{
#ifdef TS_ENGINE_EDITOR
		//Create camera's frustrum
		{
			Matrix4 projViewIMat = glm::inverse(GetProjectionViewMatrix());

			std::vector<Vector4> homogeneousFrustrumPoints = {
				projViewIMat * glm::vec4(-1, -1, -1, 1),//0
				projViewIMat * glm::vec4(-1, -1, 1, 1),//1

				projViewIMat * glm::vec4(-1, -1, 1, 1),//1
				projViewIMat * glm::vec4(-1, 1, 1, 1),//2

				projViewIMat * glm::vec4(-1, 1, 1, 1),//2
				projViewIMat * glm::vec4(-1, 1, -1, 1),//3

				projViewIMat * glm::vec4(-1, 1, -1, 1),//3
				projViewIMat * glm::vec4(-1, -1, -1, 1),//0

				projViewIMat * glm::vec4(1, -1, -1, 1),//4
				projViewIMat * glm::vec4(1, -1, 1, 1),//5

				projViewIMat * glm::vec4(1, -1, -1, 1),//4
				projViewIMat * glm::vec4(1, 1, -1, 1),//7

				projViewIMat * glm::vec4(1, 1, 1, 1),//6
				projViewIMat * glm::vec4(1, 1, -1, 1),//7

				projViewIMat * glm::vec4(1, -1, 1, 1),//5
				projViewIMat * glm::vec4(1, 1, 1, 1),//6

				projViewIMat * glm::vec4(-1, 1, -1, 1),//3
				projViewIMat * glm::vec4(1, 1, -1, 1),//7

				projViewIMat * glm::vec4(-1, -1, -1, 1),//0
				projViewIMat * glm::vec4(1, -1, -1, 1),//4

				projViewIMat * glm::vec4(-1, 1, 1, 1),//2
				projViewIMat * glm::vec4(1, 1, 1, 1),//6

				projViewIMat * glm::vec4(-1, -1, 1, 1),//1
				projViewIMat * glm::vec4(1, -1, 1, 1),//5
			};

			//Multiplying the point with inverse of mCameraNode's transform is important because it will be following mCamerNode
			for (auto& frustrumPoints : homogeneousFrustrumPoints)
			{
				frustrumPoints = glm::inverse(mCameraNode->GetTransform()->GetWorldTransformationMatrix()) * frustrumPoints;
			}

			std::vector<Vector3> nonHomogeneousFrustrumPoints(homogeneousFrustrumPoints.size());

			for (int i = 0; i < homogeneousFrustrumPoints.size(); i++)
				nonHomogeneousFrustrumPoints[i] = Vector3(homogeneousFrustrumPoints[i]) / homogeneousFrustrumPoints[i].w;

			mSceneCameraFrustrumNode = Factory::GetInstance()->InstantiateLine("SceneCameraFrustrum", nullptr, nonHomogeneousFrustrumPoints);
			mSceneCameraFrustrumNode->GetMeshes()[0]->GetMaterial()->DisableDepthTest();
		}

		//SceneCameraGuiQuad
		{
			mCameraIcon = TS_ENGINE::Texture2D::Create(Application::s_ResourcesDir.string() + "\\Gui\\CameraIcon.png");
			mCameraIcon->SetVerticalFlip(false);

			mSceneCameraGuiNode = Factory::GetInstance()->InstantiateQuad("SceneCameraGuiNode", nullptr);
			mSceneCameraGuiNode->GetMeshes()[0]->GetMaterial()->SetDiffuseMap(mCameraIcon);
			mSceneCameraGuiNode->GetMeshes()[0]->GetMaterial()->EnableAlphaBlending();//Enable transparency
			mSceneCameraGuiNode->GetTransform()->SetLocalEulerAngles(0.0, 90.0f, 0.0f);
			mSceneCameraGuiNode->GetTransform()->SetLocalScale(-1.0f, 1.0f, 1.0f);
		}		
#endif
	}


	void SceneCamera::Update(Ref<Shader> shader, float deltaTime)
	{
		mViewMatrix = mCameraNode->GetTransform()->GetWorldTransformationMatrix();
		mViewMatrix = glm::inverse(mViewMatrix);

		shader->SetVec3("u_ViewPos", mCameraNode->GetTransform()->GetLocalPosition());

		if (mIsDistanceIndependent)
		{
			shader->SetMat4("u_View", (Matrix4)((Matrix3)mViewMatrix));
		}
		else
		{
			shader->SetMat4("u_View", mViewMatrix);
			shader->SetMat4("u_Projection", mProjectionMatrix);
		}
	}

#ifdef TS_ENGINE_EDITOR
	void SceneCamera::ShowCameraGUI(Ref<Shader> shader, float deltaTime)
	{
		if (mEditorCamera)
		{
			mSceneCameraFrustrumNode->GetTransform()->Follow(mCameraNode);// Frustrum
			mSceneCameraGuiNode->GetTransform()->LookAt(mCameraNode, mEditorCamera->GetNode()->GetTransform());// CameraGUI
		}

		mSceneCameraGuiNode->Update(shader, deltaTime);
	}

	void SceneCamera::ShowFrustrumGUI(Ref<Shader> shader, float deltaTime)
	{
		mSceneCameraFrustrumNode->Update(shader, deltaTime);
	}
	

	bool SceneCamera::IsSceneCameraGuiSelected(int entityID)
	{
		if (entityID == mSceneCameraGuiNode->GetEntity()->GetEntityID())
			return true;
		else
			return false;
	}

	void SceneCamera::RefreshFrustrumGUI()
	{
		Matrix4 projViewIMat = glm::inverse(GetProjectionViewMatrix());

		std::vector<Vector4> homogeneousFrustrumPoints = {
			projViewIMat * glm::vec4(-1, -1, -1, 1),//0
			projViewIMat * glm::vec4(-1, -1, 1, 1),//1

			projViewIMat * glm::vec4(-1, -1, 1, 1),//1
			projViewIMat * glm::vec4(-1, 1, 1, 1),//2

			projViewIMat * glm::vec4(-1, 1, 1, 1),//2
			projViewIMat * glm::vec4(-1, 1, -1, 1),//3

			projViewIMat * glm::vec4(-1, 1, -1, 1),//3
			projViewIMat * glm::vec4(-1, -1, -1, 1),//0

			projViewIMat * glm::vec4(1, -1, -1, 1),//4
			projViewIMat * glm::vec4(1, -1, 1, 1),//5

			projViewIMat * glm::vec4(1, -1, -1, 1),//4
			projViewIMat * glm::vec4(1, 1, -1, 1),//7

			projViewIMat * glm::vec4(1, 1, 1, 1),//6
			projViewIMat * glm::vec4(1, 1, -1, 1),//7

			projViewIMat * glm::vec4(1, -1, 1, 1),//5
			projViewIMat * glm::vec4(1, 1, 1, 1),//6

			projViewIMat * glm::vec4(-1, 1, -1, 1),//3
			projViewIMat * glm::vec4(1, 1, -1, 1),//7

			projViewIMat * glm::vec4(-1, -1, -1, 1),//0
			projViewIMat * glm::vec4(1, -1, -1, 1),//4

			projViewIMat * glm::vec4(-1, 1, 1, 1),//2
			projViewIMat * glm::vec4(1, 1, 1, 1),//6

			projViewIMat * glm::vec4(-1, -1, 1, 1),//1
			projViewIMat * glm::vec4(1, -1, 1, 1),//5
		};

		//Multiplying the point with inverse of mCameraNode's transform is important because it will be following mCamerNode
		for (auto& frustrumPoints : homogeneousFrustrumPoints)
		{
			frustrumPoints = glm::inverse(mCameraNode->GetTransform()->GetWorldTransformationMatrix()) * frustrumPoints;
		}

		std::vector<Vertex> nonHomogeneousFrustrumVertices(homogeneousFrustrumPoints.size());
		
		for (int i = 0; i < homogeneousFrustrumPoints.size(); i++)
		{					
			Vector3 pos = Vector3(homogeneousFrustrumPoints[i]) / homogeneousFrustrumPoints[i].w;
			nonHomogeneousFrustrumVertices[i].position = Vector4(pos, 1);
		}

		mSceneCameraFrustrumNode->GetMeshes()[0]->SetVertices(nonHomogeneousFrustrumVertices);
		mSceneCameraFrustrumNode->GetMeshes()[0]->Create(DrawMode::LINE);
	}

#else
	void SceneCamera::Controls(float deltaTime)
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
#endif

	void SceneCamera::DeleteMeshes()
	{

	}
}
