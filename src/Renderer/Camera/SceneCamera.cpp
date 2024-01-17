#include "tspch.h"
#include "SceneCamera.h"
#include "Input.h"
#include <GLM/gtx/quaternion.hpp>
#include "Core/Factory.h"
#include "Renderer/MaterialManager.h"

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
				frustrumPoints = glm::inverse(mCameraNode->GetTransform()->GetGlobalTransformationMatrix()) * frustrumPoints;
			}

			std::vector<Vector3> nonHomogeneousFrustrumPoints(homogeneousFrustrumPoints.size());

			for (int i = 0; i < homogeneousFrustrumPoints.size(); i++)
				nonHomogeneousFrustrumPoints[i] = Vector3(homogeneousFrustrumPoints[i]) / homogeneousFrustrumPoints[i].w;

			mSceneCameraFrustrumNode = Factory::GetInstance()->InstantiateLine("SceneCameraFrustrum", nullptr, nonHomogeneousFrustrumPoints);
			mSceneCameraFrustrumNode->GetMeshes()[0]->GetMaterial()->DisableDepthTest();
		}

		//SceneCameraGuiQuad
		{
			mCameraIcon = TS_ENGINE::Texture2D::Create(Application::s_ResourcesDir.string() + "\\Gui\\Camera.png");
			mCameraIcon->SetVerticalFlip(false);

			mSceneCameraGuiNode = Factory::GetInstance()->InstantiateQuad("SceneCameraGuiNode", nullptr);
			mSceneCameraGuiNode->GetMeshes()[0]->GetMaterial()->SetDiffuseMap(mCameraIcon);
			mSceneCameraGuiNode->GetMeshes()[0]->GetMaterial()->EnableAlphaBlending();//Enable transparency
			mSceneCameraGuiNode->GetTransform()->SetLocalEulerAngles(0.0, 90.0f, 0.0f);
			mSceneCameraGuiNode->GetTransform()->SetLocalScale(-1.0f, 1.0f, 1.0f);
		}		
#endif
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
			frustrumPoints = glm::inverse(mCameraNode->GetTransform()->GetGlobalTransformationMatrix()) * frustrumPoints;
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

	void SceneCamera::Update(float deltaTime)
	{
		mViewMatrix = mCameraNode->GetTransform()->GetGlobalTransformationMatrix();
		mViewMatrix = glm::inverse(mViewMatrix);

		for (auto& material : MaterialManager::GetInstance()->GetAllMaterials())
		{
			material->GetShader()->SetVec3("u_ViewPos", mCameraNode->GetTransform()->GetLocalPosition());

			if (mIsDistanceIndependent)
			{
				material->GetShader()->SetMat4("u_View", (Matrix4)((Matrix3)mViewMatrix));
			}
			else
			{
				material->GetShader()->SetMat4("u_View", mViewMatrix);
				material->GetShader()->SetMat4("u_Projection", mProjectionMatrix);
			}
		}
	}

	void SceneCamera::ShowCameraGUI(float deltaTime)
	{
#ifdef TS_ENGINE_EDITOR
		if (mEditorCamera)
		{
			mSceneCameraFrustrumNode->GetTransform()->Follow(mCameraNode);// Frustrum
			mSceneCameraGuiNode->GetTransform()->LookAt(mCameraNode, mEditorCamera->GetNode()->GetTransform());// CameraGUI
		}
#endif
		mSceneCameraGuiNode->Update(deltaTime);		
	}

	void SceneCamera::ShowFrustrumGUI(float deltaTime)
	{
		mSceneCameraFrustrumNode->Update(deltaTime);
	}

	bool SceneCamera::IsSceneCameraGuiSelected(int entityID)
	{
		if (entityID == mSceneCameraGuiNode->GetEntity()->GetEntityID())
			return true;
		else
			return false;
	}

	void SceneCamera::DeleteMeshes()
	{

	}
}
