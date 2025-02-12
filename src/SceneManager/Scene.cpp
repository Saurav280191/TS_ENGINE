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
	Scene::Scene():
		mSelectedBoneId(-1),
		mName("Default"),
		mSelectedModelRootNodeId(-1),
		mAnimations({})
	{
		mSceneNode = CreateRef<Node>();		

		mCurrentSceneCameraIndex = 0;
		//m_BatchButton.RegisterClickHandler(std::bind(&ButtonHandler::OnButtonClicked, &mBatchButtonHandler, std::placeholders::_1, std::placeholders::_2));

		mSkybox = CreateRef<Skybox>();
		mSkybox->Initialize("Skybox", NodeType::SKYBOX);
	}

	Scene::~Scene()
	{
		TS_CORE_INFO("Deleting scene named: {0}", mName);

#ifdef TS_ENGINE_EDITOR
		mEditorCamera->Clear();
		mEditorCamera = nullptr;
#endif
		for (auto sceneCamera : mSceneCameras)
		{
			sceneCamera->Clear();
			sceneCamera = nullptr;
		}

		// TODO: Check why the destructors for the scene variables are not getting called?

		mSceneCameras.clear();

		Factory::GetInstance()->Clear();

		mSceneNode = nullptr;

		//mSceneNode.reset();
		//ModelLoader::GetInstance()->Flush();
	}

	void Scene::SetName(const std::string& _name)
	{
		mName = _name;
	}

	void Scene::Initialize()
	{
		mSceneNode->Initialize(mName, NodeType::SCENE);// Needs to be initialize at the end to initialize the hierarchy once
	}

	void Scene::AddEditorCamera(Ref<EditorCamera> editorCamera)
	{
#ifdef TS_ENGINE_EDITOR
		mEditorCamera = editorCamera;
#endif
	}

	void Scene::AddSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		sceneCamera->GetNode()->SetParent(mSceneNode);
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

	void Scene::Update(float _deltaTime)
	{
		mSceneNode->AnimationUpdate(_deltaTime);
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
		// Scene camera pass
		if (mSceneCameras.size() > 0)
		{
			if (mSceneCameras[mCurrentSceneCameraIndex])
			{
				Ref<Camera> sceneCamera = mSceneCameras[mCurrentSceneCameraIndex];

				UpdateCameraRT(sceneCamera, shader, deltaTime, false);

#ifdef TS_ENGINE_EDITOR
				mSceneCameras[mCurrentSceneCameraIndex]->GetFramebuffer()->Unbind();
#endif
			}
		}
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

#ifdef TS_ENGINE_EDITOR
		camera->GetFramebuffer()->Bind();
#endif

		//if(!mGammaCorrection)
		RenderCommand::SetClearColor(Vector4(0.2f, 0.3f, 0.3f, 1.0f));
		//else
			//TS_ENGINE::RenderCommand::SetClearColor(Vector4(pow(0.2f, mGammaValue), pow(0.3f, mGammaValue), pow(0.3f, mGammaValue), 1.0f));

		RenderCommand::Clear();

		// Clear entity ID attachment to -1
#ifdef TS_ENGINE_EDITOR
		camera->GetFramebuffer()->ClearAttachment(1, -1);
#endif

		//mCurrentShader->SetBool("u_Gamma", mGammaCorrection);
		//mCurrentShader->SetFloat("u_GammaValue", mGammaValue);
		//mCurrentShader->SetBool("u_Hdr", mHdr);
		//mCurrentShader->SetFloat("u_HdrExposure", mHdrExposure);
		//mDirectionalLight->SetCommonParams(mCurrentShader, mDirectionalLight->GetNode()->GetTransform()->GetLocalPosition(),
		//mDirectionalLight->GetNode()->GetTransform()->GetForward(), Vector3(0.5f), Vector3(0.5f), Vector3(0.5f));

		// Render Skybox
		camera->SetIsDistanceIndependent(true);				// Make Camera Distance Independent For Rendering Skybox
		camera->Update(shader, deltaTime);					// Camera's View And Projection Matrix Updates 
		mSkybox->Render();									// Render Skybox
		camera->SetIsDistanceIndependent(false);			// Disable Camera Distance Independent For Rendering Other World Objects

		TS_CORE_ASSERT(mSceneNode);							// Make Sure Scene Node Is Set

		camera->Update(shader, deltaTime);					// Camera's View And Projection Matrix Updates 
		
		mSceneNode->Update(shader, deltaTime);				// Updates Shader Parameters And Renders Scene Hierarchy

		//if(mSelectedModelRootNodeEntityId != -1)			// Check mSelectedModelRootNodeEntityId match on CPU
		//	mSceneNode->CheckSelectedModelRootNodeEntityId(
		// mSelectedModelRootNodeEntityId);

		// Set selected modelCopyId
		shader->SetInt("u_SelectedModelRootNodeEntityId",	// Pass selected modelCopyId to shader
			mSelectedModelRootNodeId);

		// Set selected bone Id
		shader->SetInt("u_SelectedBoneId",					// Pass selected bone to shader
			mSelectedBoneId);

		// Set bone influence view
		shader->SetInt("u_BoneInfluence",					// Pass bone influence to shader
			(int)Application::GetInstance().mBoneInfluence);
	}

#ifdef TS_ENGINE_EDITOR
	int Scene::GetSkyboxNodeId()
	{
		return mSkybox->mId;
	}
#endif

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
				TS_CORE_INFO("Switched current scene camera to: {0}", mSceneCameras[i]->GetNode()->mName.c_str());
				mCurrentSceneCameraIndex = i;
				return;
			}
		}
	}
	
#ifdef TS_ENGINE_EDITOR
	void Scene::ShowSceneCameraGUI(Ref<Shader> shader, float deltaTime)
	{
		for (auto& sceneCamera : mSceneCameras)
			sceneCamera->ShowCameraGUI(shader, deltaTime);//Render Scene camera's GUI

		if(mSceneCameras.size() > 0)
			mSceneCameras[mCurrentSceneCameraIndex]->ShowFrustrumGUI(shader, deltaTime);
	}
#endif

	void Scene::AddAnimation(Ref<Animation> _animation)
	{
		mAnimations.insert({ _animation->GetName(), _animation });
	}
}
