#include "tspch.h"
#include "SceneCamera.h"
#include "Input.h"
#include <GLM/gtx/quaternion.hpp>

namespace TS_ENGINE
{
#ifdef TS_ENGINE_EDITOR
	SceneCamera::SceneCamera(const std::string& name, Ref<Camera> editorCamera)
	{
		mName = name;
		mNode->SetName(name);
		mCameraType = Type::SCENECAMERA;
		mEditorCamera = editorCamera;
	}
#else
	SceneCamera::SceneCamera(const std::string& name) 
		: Camera(name)
	{
		mName = name;
		mCameraType = Type::SCENECAMERA;

		//mSkyboxShader = TS_ENGINE::Shader::Create("HDRLighting", "HDRLighting.vert", "HDRLighting.frag");
		//mSkyboxMat = CreateRef<TS_ENGINE::Material>("SkyboxMaterial", mSkyboxShader);//Create default material

		//mCurrentShader = mSkyboxShader;
	}
#endif

	void SceneCamera::Initialize()
	{
		mEntityID = EntityManager::GetInstance()->Instantiate(mName, mEntityType);

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

			mFrustrumLine = CreateRef<TS_ENGINE::Line>("FrustrumLine");
			//mFrustrumLine->SetMaterial(mDefaultMat);
			mFrustrumLine->SetColor(1, 1, 1);
			mFrustrumLine->DisableDepthTest();
			mFrustrumLine->Create(vertices);

			mFrustrumLine->GetNode()->AttachObject(mFrustrumLine);
			mFrustrumLine->GetNode()->SetName("FrustrumLine");

			//mFrustrumLine->GetNode()->HideInEditor();//Hides the node in hierarchy			
			//mNode->AddChild(mFrustrumLine->GetNode());
		}

		//SceneCameraGuiQuad
		{
			mCameraIcon = TS_ENGINE::Texture2D::Create("Resources\\Gui\\Camera.png");
			mCameraIcon->SetVerticalFlip(false);

			mSceneCameraGui = CreateRef<TS_ENGINE::Quad>("CameraGui");
			mSceneCameraGui->EnableAlphaBlending();//Enable transparency
			//mSceneCameraGui->SetMaterial(mDefaultMat);
			mSceneCameraGui->SetColor(1, 1, 1);
			mSceneCameraGui->SetTexture(mCameraIcon);
			mSceneCameraGui->Create();

			mSceneCameraGui->GetNode()->AttachObject(mSceneCameraGui);
			mSceneCameraGui->GetNode()->SetName("CameraGuiNode");
			mSceneCameraGui->GetNode()->GetTransform()->SetLocalEulerAngles(0.0, 90.0f, 0.0f);
			mSceneCameraGui->GetNode()->GetTransform()->SetLocalScale(-1.0f, 1.0f, 1.0f);

			//mSceneCameraGui->GetNode()->HideInEditor();//Hides the node in hierarchy
			//mNode->AddChild(mSceneCameraGui->GetNode());
		}
#endif
	}

	void SceneCamera::Update(Ref<Shader> shader, float deltaTime)
	{
#ifdef TS_ENGINE_EDITOR
		if (mEditorCamera)
		{
			mSceneCameraGui->GetNode()->GetTransform()->LookAt(mNode.get(), mEditorCamera->GetNode()->GetTransform());
			mFrustrumLine->GetNode()->GetTransform()->Follow(mNode);
		}
#endif

		mViewMatrix = mNode->GetTransform()->GetTransformationMatrix();
		mViewMatrix = glm::inverse(mViewMatrix);

		shader->SetVec3("u_ViewPos", mNode->GetTransform()->GetLocalPosition());
		shader->SetMat4("u_View", mViewMatrix);
		shader->SetMat4("u_Projection", mProjectionMatrix);
	}

	bool SceneCamera::IsSceneCameraGuiSelected(int entityID)
	{
		if (entityID == mSceneCameraGui->GetEntityID())
			return true;
		else
			return false;
	}

	void SceneCamera::DeleteMeshes()
	{
	}

	void SceneCamera::RenderGui(Ref<Shader> shader, float deltaTime)
	{
		mFrustrumLine->GetNode()->Update(shader, deltaTime);
		mSceneCameraGui->GetNode()->Update(shader, deltaTime);
	}

	//void SceneCamera::CheckIfSelected(Ref<TS_ENGINE::Node>& hoveredOnNode)
	//{
	//	if (hoveredOnNode == mSceneCameraGui->GetNode()//Don't select mSceneCameraGui 
	//		|| hoveredOnNode == mFrustrumLine->GetNode())//or mFrustrumLine
	//	{
	//		hoveredOnNode = this->mNode;//Select it's parent node instead
	//	}
	//}
}
