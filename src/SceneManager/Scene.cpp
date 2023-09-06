#include "tspch.h"
#include "SceneManager/Scene.h"
#include "Primitive/Quad.h"
#include "Primitive/Cube.h"
#include "Primitive/Model.h"
#include "Core/ModelLoader.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

#include "Renderer/RenderCommand.h"
#include "Core/Factory.h"

namespace TS_ENGINE
{
	Scene::Scene(std::string name, Ref<EditorCamera> editorCamera, Ref<SceneCamera> sceneCamera)
		//: m_BatchingEnabled(false)
	{
		mSceneNode = CreateRef<Node>();
		mSceneNode->SetNodeRef(mSceneNode);
		mEditorCamera = editorCamera;
		mCurrentSceneCameraIndex = 0;
		//m_BatchButton.RegisterClickHandler(std::bind(&ButtonHandler::OnButtonClicked, &mBatchButtonHandler, std::placeholders::_1, std::placeholders::_2));
		
#pragma region DefaultNewScene
		mSkyboxNode = Factory::GetInstance()->InstantiateSphere("SkyboxSphere", nullptr);
		mSkyboxNode->GetMeshes()[0]->GetMaterial()->SetDiffuseMap(TS_ENGINE::Texture2D::Create(Application::s_AssetsDir.string() + "\\Textures\\Skybox\\industrial_sunset_puresky.jpg"));
		mSkyboxNode->GetTransform()->SetLocalScale(1600.0f, 1600.0f, 1600.0f);
		mSkyboxNode->GetTransform()->SetLocalEulerAngles(90.0f, 235.0f, 0.0f);
		mSkyboxNode->InitializeTransformMatrices();

		sceneCamera->GetNode()->SetParent(mSceneNode);
		mSceneCameras.push_back(sceneCamera);
#pragma endregion

		mSceneNode->Initialize(name, EntityType::SCENE);//Needs to be done at the end to initialize the hierarchy once
	}

	Scene::Scene(std::string name, Ref<EditorCamera> editorCamera, std::vector<Ref<SceneCamera>> sceneCameras)
		//: m_BatchingEnabled(false)
	{
		mSceneNode = CreateRef<Node>();
		mSceneNode->SetNodeRef(mSceneNode);
		mEditorCamera = editorCamera;
		mCurrentSceneCameraIndex = 0;
		//m_BatchButton.RegisterClickHandler(std::bind(&ButtonHandler::OnButtonClicked, &mBatchButtonHandler, std::placeholders::_1, std::placeholders::_2));

#pragma region DefaultNewScene
		mSkyboxNode = Factory::GetInstance()->InstantiateSphere("SkyboxSphere", nullptr);
		mSkyboxNode->GetMeshes()[0]->GetMaterial()->SetDiffuseMap(TS_ENGINE::Texture2D::Create(Application::s_AssetsDir.string() + "\\Textures\\Skybox\\industrial_sunset_puresky.jpg"));
		mSkyboxNode->GetTransform()->SetLocalScale(1600.0f, 1600.0f, 1600.0f);
		mSkyboxNode->GetTransform()->SetLocalEulerAngles(90.0f, 235.0f, 0.0f);
		mSkyboxNode->InitializeTransformMatrices();

		//Add multiple camera
		for (auto& sceneCamera : sceneCameras)
		{
			sceneCamera->GetNode()->SetParent(mSceneNode);
			mSceneCameras.push_back(sceneCamera);
		}
#pragma endregion

		mSceneNode->Initialize(name, EntityType::SCENE);//Needs to be done at the end to initialize the hierarchy once
	}

	Scene::~Scene()
	{
		mSceneNode.reset();
		//mSceneNode->Destroy();
		//mSceneNode = nullptr;
		//delete mSceneNode;
	}

	/*void Scene::OnBatched()
	{
		mSceneNode->RemoveAllChildren();
		Ref<Node> batchedNode = Batcher::GetInstance()->GetBatchedNode();
		mSceneNode->AddChild(mSceneNode, batchedNode);
		batchedNode->SetParentNode(mSceneNode);
	}*/

	/*void Scene::OnUnBatched()
	{
		mSceneNode->RemoveChild(Batcher::GetInstance()->GetBatchedNode());
		Batcher::GetInstance()->GetBatchedNode()->GetTransform()->Reset();
	}*/

	void Scene::Render(Ref<Shader> shader, float deltaTime)
	{
		//Scene camera pass
		if (mSceneCameras[mCurrentSceneCameraIndex])
		{
			UpdateCameraRT(mSceneCameras[mCurrentSceneCameraIndex], shader, deltaTime, false);
			mSceneCameras[mCurrentSceneCameraIndex]->GetFramebuffer()->Unbind();
		}
	}

	void Scene::SetCurrentSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		for (int i = 0; i < mSceneCameras.size(); i++)
		{
			if (mSceneCameras[i] == sceneCamera)
			{
				mCurrentSceneCameraIndex = i;
				return;
			}
		}
	}

	void Scene::SwitchToAnotherSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		for (int i = 0; i < mSceneCameras.size(); i++)
		{
			if (mSceneCameras[i] != sceneCamera)
			{
				TS_CORE_INFO("Switched current scene camera to: {0}", mSceneCameras[i]->GetNode()->GetEntity()->GetName().c_str());
				mCurrentSceneCameraIndex = i;
				return;
			}
		}
	}
	
	void Scene::AddSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		mSceneCameras.push_back(sceneCamera);
	}

	void Scene::RemoveSceneCamera(Ref<SceneCamera> sceneCamera) 
	{
		for (int i = 0; i < mSceneCameras.size(); i++)
		{
			if (mSceneCameras[i] == sceneCamera)
			{
				mSceneCameras.erase(mSceneCameras.begin() + i);
				return;
			}
		}
	}

	void Scene::ShowSceneCameraGUI(Ref<Shader> shader, float deltaTime)
	{
		for (auto& sceneCamera : mSceneCameras)
			sceneCamera->ShowCameraGUI(shader, deltaTime);//Render Scene camera's GUI

		mSceneCameras[mCurrentSceneCameraIndex]->ShowFrustrumGUI(shader, deltaTime);
	}

	void Scene::UpdateCameraRT(Ref<Camera> camera, Ref<Shader> shader, float deltaTime, bool isEditorCamera)
	{
		// Resize
		//if (TS_ENGINE::FramebufferSpecification spec = camera->GetFramebuffer()->GetSpecification();
		//	mViewportPanelSize.x > 0.0f && mViewportPanelSize.y > 0.0f && // zero sized framebuffer is invalid
		//	(spec.Width != mViewportPanelSize.x || spec.Height != mViewportPanelSize.y))
		//{
		//	camera->GetFramebuffer()->Resize((uint32_t)mViewportPanelSize.x, (uint32_t)mViewportPanelSize.y);
		//}

		camera->GetFramebuffer()->Bind();

		//if(!mGammaCorrection)
		RenderCommand::SetClearColor(Vector4(0.2f, 0.3f, 0.3f, 1.0f));
		//else
			//TS_ENGINE::RenderCommand::SetClearColor(Vector4(pow(0.2f, mGammaValue), pow(0.3f, mGammaValue), pow(0.3f, mGammaValue), 1.0f));

		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		camera->GetFramebuffer()->ClearAttachment(1, -1);

		//mCurrentShader->SetBool("u_Gamma", mGammaCorrection);
		//mCurrentShader->SetFloat("u_GammaValue", mGammaValue);
		//mCurrentShader->SetBool("u_Hdr", mHdr);
		//mCurrentShader->SetFloat("u_HdrExposure", mHdrExposure);
		//mDirectionalLight->SetCommonParams(mCurrentShader, mDirectionalLight->GetNode()->GetTransform()->GetLocalPosition(),
		//	mDirectionalLight->GetNode()->GetTransform()->GetForward(), Vector3(0.5f), Vector3(0.5f), Vector3(0.5f));

		// Camera And Skybox Render (To render skybox without distance dependency)		
		camera->SetIsDistanceIndependent(true);
		camera->Update(shader, deltaTime);				
		mSkyboxNode->Update(shader, deltaTime);

		// Camera And Scene Render		
		camera->SetIsDistanceIndependent(false);
		camera->Update(shader, deltaTime);

		// Scene-Tree Render
		if (mSceneNode)
			mSceneNode->Update(shader, deltaTime);
		else
			TS_CORE_ERROR("Scene node not set");

	}
}
