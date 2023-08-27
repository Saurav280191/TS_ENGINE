#include "tspch.h"
#include "Factory.h"
#include "Utils/Utility.h"

namespace TS_ENGINE
{
	Factory* Factory::mInstance = NULL;
	Factory* Factory::GetInstance()
	{
		if (mInstance == NULL)
		{
			mInstance = new Factory();
		}

		return mInstance;
	}
	
	Ref<SceneCamera> Factory::InstantitateSceneCamera(const std::string& name, Scene* scene)
	{
#ifdef TS_ENGINE_EDITOR
		TS_CORE_ASSERT(scene->GetEditorCamera());
		Ref<SceneCamera> sceneCamera = CreateRef<SceneCamera>("SceneCamera", scene->GetEditorCamera());
#else
		Ref<SceneCamera> sceneCamera = CreateRef<SceneCamera>("SceneCamera");
#endif
		sceneCamera->SetPerspective(Camera::Perspective(45.0f, 1.77f, 1.0f, 20.0f));
		sceneCamera->CreateFramebuffer(800, 600);
		sceneCamera->Initialize();
		sceneCamera->GetNode()->SetName(name);
		sceneCamera->GetNode()->SetParent(scene->GetSceneNode());
		return sceneCamera;
	}

	Ref<Node> Factory::InstantiateQuad(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> quadNode = CreateRef<Node>();
		quadNode->SetNodeRef(quadNode);
		quadNode->SetName(name);
		quadNode->AddMesh(CreateRef<TS_ENGINE::Quad>()->GetMesh());
		Ref<Mesh> mesh = quadNode->GetMeshes()[0];
		quadNode->SetParent(parentNode);
		return quadNode;
	}

	Ref<Node> Factory::InstantiateCube(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> cubeNode = CreateRef<Node>();
		cubeNode->SetNodeRef(cubeNode);
		cubeNode->SetName(name);
		cubeNode->AddMesh(CreateRef<TS_ENGINE::Cube>()->GetMesh());
		Ref<Mesh> mesh = cubeNode->GetMeshes()[0];
		cubeNode->SetParent(parentNode);
		return cubeNode;
	}

	Ref<Node> Factory::InstantiateSphere(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> sphereNode = CreateRef<Node>();
		sphereNode->SetNodeRef(sphereNode);
		sphereNode->SetName(name);
		sphereNode->AddMesh(CreateRef<TS_ENGINE::Sphere>()->GetMesh());
		Ref<Mesh> mesh = sphereNode->GetMeshes()[0];
		sphereNode->SetParent(parentNode);
		return sphereNode;
	}

	// TODO: Add code for Cylinder and Cone generation

	Ref<Node> Factory::InstantiateModel(const std::string& modelPath, Ref<Node> parentNode)
	{
		Ref<Model> model = ModelLoader::GetInstance()->LoadModel(modelPath);
		Ref<Node> mModelNode = model->GetRootNode();
		mModelNode->SetParent(parentNode);
		return mModelNode;
	}

	//Ref<Light> Factory::CreateLight(Light::Type type)
	//{
	//	switch (type)
	//	{
	//	case Light::Type::DIRECTIONAL:
	//		//mDirectionalLight = CreateRef<TS_ENGINE::Light>();
	//		//mDirectionalLight->GetNode()->GetTransform()->SetLocalPosition(0.0f, 3.0f, 0.0f);
	//		//mDirectionalLight->GetNode()->GetTransform()->SetLocalEulerAngles(45.0f, 0.0f, 0.0f);
	//		//mDirectionalLight->GetNode()->SetName("Directional Light");
	//		//mDirectionalLight->GetNode()->AttachObject(mDirectionalLight);
	//		//mDirectionalLight->GetNode()->AttachLight(mDirectionalLight);
	//		return nullptr;
	//		break;
	//	case Light::Type::POINT:
	//		return nullptr;
	//		break;
	//	case Light::Type::SPOT:
	//		return nullptr;
	//		break;
	//	}
	//	return nullptr;
	//}
}
