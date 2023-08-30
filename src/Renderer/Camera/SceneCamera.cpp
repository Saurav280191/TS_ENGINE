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
		//mCameraNode->SetName(name);
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

			std::vector<Vector3> nonHomogeneousFrustrumPoints(homogeneousFrustrumPoints.size());
			
			for (int i = 0; i < homogeneousFrustrumPoints.size(); i++)
				nonHomogeneousFrustrumPoints[i] = Vector3(homogeneousFrustrumPoints[i]) / homogeneousFrustrumPoints[i].w;
			
			mSceneCameraFrustrumNode = Factory::GetInstance()->InstantiateLine("SceneCameraFrustrum", nullptr, nonHomogeneousFrustrumPoints);
			mSceneCameraFrustrumNode->GetMeshes()[0]->GetMaterial()->DisableDepthTest();
		}

		//SceneCameraGuiQuad
		{
			mCameraIcon = TS_ENGINE::Texture2D::Create("Resources\\Gui\\Camera.png");
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
#ifdef TS_ENGINE_EDITOR
		if (mEditorCamera)
		{
			mSceneCameraGuiNode->GetTransform()->LookAt(mCameraNode, mEditorCamera->GetNode()->GetTransform());
			mSceneCameraFrustrumNode->GetTransform()->Follow(mCameraNode);
		}
#endif

		mViewMatrix = mCameraNode->GetTransform()->GetTransformationMatrix();
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

	void SceneCamera::RenderGui(Ref<Shader> shader, float deltaTime)
	{
		mSceneCameraFrustrumNode->Update(shader, deltaTime);
		mSceneCameraGuiNode->Update(shader, deltaTime);		
	}
}
