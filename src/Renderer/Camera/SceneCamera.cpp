#include "tspch.h"
#include "SceneCamera.h"
#include "Input.h"
#include <GLM/gtx/quaternion.hpp>

namespace TS_ENGINE {

#ifdef TS_ENGINE_EDITOR
	SceneCamera::SceneCamera(const std::string& name, Ref<Camera> editorCamera) : 
		Camera(name)
	{
		mCameraNode = CreateRef<Node>(name);
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

			std::vector<Vector4> frustrumPoints = {
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

			std::vector<Vector3> vertices(frustrumPoints.size());
			
			for (int i = 0; i < frustrumPoints.size(); i++)
				vertices[i] = Vector3(frustrumPoints[i]) / frustrumPoints[i].w;

			mFrustrumLineNode = CreateRef<Node>("SceneCameraFrustrumLine");
			mFrustrumLine = CreateRef<TS_ENGINE::Line>();
			mLineMesh = mFrustrumLine->GetMesh(vertices);
			mLineMesh->GetMaterial()->DisableDepthTest();
			mFrustrumLineNode->AddMesh(mLineMesh);
			mFrustrumLineNode->InitializeTransformMatrices();
		}

		//SceneCameraGuiQuad
		{
			mCameraIcon = TS_ENGINE::Texture2D::Create("Resources\\Gui\\Camera.png");
			mCameraIcon->SetVerticalFlip(false);

			mSceneCameraGuiNode = CreateRef<Node>("SceneCameraGuiNode");
			
			Ref<Mesh> sceneCameraGuiMesh = CreateRef<TS_ENGINE::Quad>()->GetMesh();
			sceneCameraGuiMesh->GetMaterial()->EnableAlphaBlending();//Enable transparency
			sceneCameraGuiMesh->GetMaterial()->SetDiffuseMap(mCameraIcon);
			mSceneCameraGuiNode->AddMesh(sceneCameraGuiMesh);
			mSceneCameraGuiNode->GetTransform()->SetLocalEulerAngles(0.0, 90.0f, 0.0f);
			mSceneCameraGuiNode->GetTransform()->SetLocalScale(-1.0f, 1.0f, 1.0f);
			mSceneCameraGuiNode->InitializeTransformMatrices();
		}
#endif
	}

	void SceneCamera::Update(Ref<Shader> shader, float deltaTime)
	{
#ifdef TS_ENGINE_EDITOR
		if (mEditorCamera)
		{
			mSceneCameraGuiNode->GetTransform()->LookAt(mCameraNode.get(), mEditorCamera->GetNode()->GetTransform());
			mFrustrumLineNode->GetTransform()->Follow(mCameraNode);
		}
#endif

		mViewMatrix = mCameraNode->GetTransform()->GetTransformationMatrix();
		mViewMatrix = glm::inverse(mViewMatrix);

		shader->SetVec3("u_ViewPos", mCameraNode->GetTransform()->GetLocalPosition());
		shader->SetMat4("u_View", mViewMatrix);
		shader->SetMat4("u_Projection", mProjectionMatrix);
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
		mFrustrumLineNode->Update(shader, deltaTime);
		mSceneCameraGuiNode->Update(shader, deltaTime);
	}
}
