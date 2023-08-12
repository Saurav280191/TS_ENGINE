#include "tspch.h"
#include "Factory.h"

namespace TS_ENGINE
{
	Factory* Factory::mInstance = NULL;
	Factory* Factory::GetInstance()
	{
		if (mInstance == NULL)
			mInstance = new Factory();
		return mInstance;
	}

	Ref<GameObject> Factory::CreateGameObject(PrimitiveType type)
	{
		switch (type)
		{
		case PrimitiveType::QUAD:		
		{
			Ref<TS_ENGINE::Quad> quad = CreateRef<TS_ENGINE::Quad>("New Quad");
			//quad->SetMaterial(mDefaultMat);
			quad->Create();
			quad->SetName("New Quad");
			quad->GetNode()->AttachObject(quad);
			//quad->GetNode()->AttachGameObject(quad);
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->AddChild(quad->GetNode());
			return quad;
		}
		break;

		case PrimitiveType::CUBE:
		{
			Ref<TS_ENGINE::Cube> cube = CreateRef<TS_ENGINE::Cube>("New Cube");
			//cube->SetMaterial(mDefaultMat);
			cube->Create();
			cube->SetName("New Cube");
			cube->GetNode()->AttachObject(cube);
			//cube->GetNode()->AttachGameObject(cube);
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->AddChild(cube->GetNode());
			return cube;
		}
		break;

		case PrimitiveType::SPHERE:
		{
			Ref<TS_ENGINE::Sphere> sphere = CreateRef<TS_ENGINE::Sphere>("New Sphere");
			//sphere->SetMaterial(mDefaultMat);
			sphere->Create();
			sphere->SetName("New Sphere");
			sphere->GetNode()->AttachObject(sphere);
			//sphere->GetNode()->AttachGameObject(sphere);
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->AddChild(sphere->GetNode());
			return sphere;
		}
		break;

		case PrimitiveType::CONE:
		{
			//Ref<TS_ENGINE::Cone> cone = CreateRef<TS_ENGINE::Cone>("New Cone");
			//cone->SetMaterial(mDefaultMat);
			//cone->Create();
			//cone->GetNode()->AttachObject(cone);
			//cone->GetNode()->AttachGameObject(cone);
			//return cone;
		}
		break;

		case PrimitiveType::CYLINDER:
		{
			//Ref<TS_ENGINE::Cylinder> cylinder = CreateRef<TS_ENGINE::Cylinder>("New Cylinder");
			//cylinder->SetMaterial(mDefaultMat);
			//cylinder->Create();
			//cylinder->GetNode()->AttachObject(cylinder);
			//cylinder->GetNode()->AttachGameObject(cylinder);
			//return cylinder;
		}
		break;

		case PrimitiveType::MODEL:
		{
			auto model = TS_ENGINE::ModelLoader::GetInstance()->LoadModel("Assets\\Models", "monk_character.glb");
			model->SetName("monk_character");
			model->GetNode()->AttachObject(model);
			//model->GetNode()->AttachGameObject(model);
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->AddChild(model->GetNode());
			return model;
		}
		break;
		}

		return nullptr;
	}
	
	void Factory::ChangeMeshForNode(Ref<Node> node, int primitiveIndex)
	{
		switch ((PrimitiveType)primitiveIndex)
		{
		case PrimitiveType::QUAD:
		{
			//Delete meshes
			node->GetAttachedObject()->DeleteMeshes();
			//Remove from scene hierarchy
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->RemoveChild(node);

			//Create new quad
			Ref<TS_ENGINE::Quad> quad = CreateRef<TS_ENGINE::Quad>(node->GetName());

			//Create mesh and material
			quad->Create();
			//quad->SetMaterial(mDefaultMat);

			//Destroy the node for new quad
			//quad->DestroyNode();
			quad->ReplaceNode(node);

			node->AttachObject(quad);
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->AddChild(node);
		}
		break;

		case PrimitiveType::CUBE:
		{
			//Destroy GameObject
			node->GetAttachedObject()->DeleteMeshes();
			//Remove from scene hierarchy
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->RemoveChild(node);

			//Create new quad
			Ref<TS_ENGINE::Cube> cube = CreateRef<TS_ENGINE::Cube>(node->GetName());

			//Create mesh and material
			cube->Create();
			//cube->SetMaterial(mDefaultMat);

			//Destroy the node for new quad
			//cube->DestroyNode();
			cube->ReplaceNode(node);

			node->AttachObject(cube);
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->AddChild(node);
		}
		break;

		case PrimitiveType::SPHERE:
		{
			//Destroy GameObject
			node->GetAttachedObject()->DeleteMeshes();
			//Remove from scene hierarchy
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->RemoveChild(node);

			//Create new quad
			Ref<TS_ENGINE::Sphere> sphere = CreateRef<TS_ENGINE::Sphere>(node->GetName());

			//Create mesh and material
			sphere->Create();
			//sphere->SetMaterial(mDefaultMat);

			//Destroy the node for new quad
			//sphere->DestroyNode();
			sphere->ReplaceNode(node);

			node->AttachObject(sphere);
			SceneManager::GetInstance()->GetCurrentScene()->GetSceneNode()->AddChild(node);
		}
		break;

		case PrimitiveType::CONE:
		{
			//Only destroy GameObject
			node->GetAttachedObject()->DeleteMeshes();
		}
		break;

		case PrimitiveType::CYLINDER:
		{
			//Only destroy GameObject
			node->GetAttachedObject()->DeleteMeshes();
		}
		break;

		case PrimitiveType::MODEL:
		{
			//Only destroy GameObject
			node->GetAttachedObject()->DeleteMeshes();
		}
		break;

		case PrimitiveType::EMPTY:
		{
			//Only destroy GameObject
			node->GetAttachedObject()->DeleteMeshes();
		}
		break;

		}
	}

#ifdef TS_ENGINE_EDITOR
	Ref<SceneCamera> Factory::CreateSceneCamera(Ref<Camera> editorCamera)
	{
		Ref<TS_ENGINE::SceneCamera> sceneCamera = CreateRef<TS_ENGINE::SceneCamera>("New SceneCamera", editorCamera);	
		//sceneCamera->GetNode()->AttachCamera(sceneCamera);
		return sceneCamera;		
	}
#else
	Ref<SceneCamera> Factory::CreateSceneCamera()
	{
		Ref<TS_ENGINE::SceneCamera> sceneCamera = CreateRef<TS_ENGINE::SceneCamera>("New SceneCamera");
		sceneCamera->GetNode()->AttachCamera(sceneCamera);
		sceneCamera->Initialize();
		return sceneCamera;
	}
#endif

	Ref<Light> Factory::CreateLight(Light::Type type)
	{
		switch (type)
		{
		case Light::Type::DIRECTIONAL:		
			//mDirectionalLight = CreateRef<TS_ENGINE::Light>();
			//mDirectionalLight->GetNode()->GetTransform()->SetLocalPosition(0.0f, 3.0f, 0.0f);
			//mDirectionalLight->GetNode()->GetTransform()->SetLocalEulerAngles(45.0f, 0.0f, 0.0f);
			//mDirectionalLight->GetNode()->SetName("Directional Light");
			//mDirectionalLight->GetNode()->AttachObject(mDirectionalLight);
			//mDirectionalLight->GetNode()->AttachLight(mDirectionalLight);
			return nullptr;
			break;
		case Light::Type::POINT:
			return nullptr;
			break;
		case Light::Type::SPOT:
			return nullptr;
			break;
		}

		return nullptr;
	}

}
