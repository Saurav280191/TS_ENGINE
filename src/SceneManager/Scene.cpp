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

namespace TS_ENGINE
{
	Scene::Scene(std::string name, Ref<Camera> editorCamera)
		//: m_BatchingEnabled(false)
	{
		//mSceneNode = CreateRef<Node>(name);
		mSceneNode = new Node(name);
		mEditorCamera = editorCamera;
		//mSceneNode->SetName(name);
		//m_BatchButton.RegisterClickHandler(std::bind(&ButtonHandler::OnButtonClicked, &mBatchButtonHandler, std::placeholders::_1, std::placeholders::_2));
		
#pragma region DummyScene	
		//Scene Camera
		Ref<TS_ENGINE::SceneCamera> defaultSceneCamera = CreateRef<TS_ENGINE::SceneCamera>("SceneCamera", mEditorCamera);
		defaultSceneCamera->GetNode()->GetTransform()->SetLocalPosition(7.156f, 2.951f, 8.770f);
		defaultSceneCamera->GetNode()->GetTransform()->SetLocalEulerAngles(-13.235f, 38.064f, 0.0f);
		defaultSceneCamera->SetPerspective(TS_ENGINE::Camera::Perspective(45.0f, 1.77f, 1.0f, 20.0f));
		defaultSceneCamera->CreateFramebuffer(800, 600);//Create framebuffer for sceneCamera
		defaultSceneCamera->Initialize();
		mSceneNode->AddChild(defaultSceneCamera->GetNode().get());		

		//Default ground
		mGroundNode = CreateRef<TS_ENGINE::Node>("Ground");
		mGroundNode->GetTransform()->SetLocalEulerAngles(-90.0f, 0.0f, 0.0f);
		mGroundNode->GetTransform()->SetLocalScale(10.0f, 10.0f, 10.0f);
		mGroundNode->AddMesh(CreateRef<TS_ENGINE::Quad>()->GetMesh());
		Ref<Mesh> mesh = mGroundNode->GetMeshes()[0];
		mesh->GetMaterial()->SetDiffuseMap(TS_ENGINE::Texture2D::Create("Assets\\Textures\\raw_plank_wall_diff_4k.jpg"));
		mesh->GetMaterial()->SetDiffuseMapTiling(Vector2(2, 2));
		mSceneNode->AddChild(mGroundNode.get());
		
		//Cube
		mCubeNode = CreateRef<TS_ENGINE::Node>("Cube");
		mCubeNode->AddMesh(CreateRef<TS_ENGINE::Cube>()->GetMesh());
		mCubeNode->GetMeshes()[0]->GetMaterial()->SetDiffuseMap(TS_ENGINE::Texture2D::Create("Assets\\Textures\\crate.png"));	
		mCubeNode->SetParent(mSceneNode);
		mCubeNode->GetTransform()->SetLocalPosition(2.75f, 0.312f, 0.0f);		
		mCubeNode->GetTransform()->SetLocalScale(0.62f, 0.62f, 0.62f);
		
		//Cube1
		mCube1Node = CreateRef<TS_ENGINE::Node>("Cube1");
		mCube1Node->AddMesh(CreateRef<TS_ENGINE::Cube>()->GetMesh());
		mCube1Node->GetMeshes()[0]->GetMaterial()->SetAmbientColor(Vector4(1, 0, 0, 1));
		mCube1Node->SetParent(mCubeNode.get());
		mCube1Node->GetTransform()->SetLocalPosition(1.0f, 1.0f, -1.0f);
		mCube1Node->GetTransform()->SetLocalScale(0.3f, 0.3f, 0.3f);
		mCube1Node->GetTransform()->SetLocalEulerAngles(30.0f, 60.0f, 10.0f);

		//Ref<Model> model = ModelLoader::GetInstance()->LoadModel("D:/Documents/ThinkSideways/WorkInProgress/TS_ENGINE Related/TS_ENGINE_Editor/Assets/Models/LamborginiAventador/Chassis.obj");
		//Ref<Model> model = ModelLoader::GetInstance()->LoadModel("D:/Documents/ThinkSideways/WorkInProgress/TS_ENGINE Related/TS_ENGINE_Editor/Assets/Models/buster_drone.glb");
		Ref<Model> model = ModelLoader::GetInstance()->LoadModel("D:/Downloads/Ely By K.Atienza.fbx");
		mModelNode = model->GetRootNode();
		mModelNode->AddChild(model->GetRootNode().get());
		mModelNode->SetParent(mSceneNode);
		mModelNode->GetTransform()->SetLocalScale(0.01f, 0.01f, 0.01f);

		// Skybox
		{
			mSkyboxNode = CreateRef<TS_ENGINE::Node>("Skybox");
			mSkyboxNode->AddMesh(CreateRef<TS_ENGINE::Sphere>()->GetMesh());
			mSkyboxNode->GetMeshes()[0]->GetMaterial()->SetDiffuseMap(TS_ENGINE::Texture2D::Create("Assets\\Textures\\industrial_sunset_puresky.jpg"));
			mSkyboxNode->GetTransform()->SetLocalScale(80000.0f, 80000.0f, 80000.0f);
			mSkyboxNode->GetTransform()->SetLocalEulerAngles(90.0f, 235.0f, 0.0f);
			mSkyboxNode->InitializeTransformMatrices();
		}

		mCurrentSceneCamera = defaultSceneCamera;//Current Scene Camera		
		mSceneNode->InitializeTransformMatrices();//Needs to be done at the end to initialize the hierarchy once
#pragma endregion 
	}

	Scene::~Scene()
	{
		mSceneNode->Destroy();
		//mSceneNode.reset();
		mSceneNode = nullptr;
		delete mSceneNode;
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
		if (mCurrentSceneCamera)
		{
			UpdateCameraRT(mCurrentSceneCamera, shader, deltaTime, false);
			mCurrentSceneCamera->GetFramebuffer()->Unbind();
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

		// Camera And Skybox Render		
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
