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

	Ref<GameObject> Factory::CreateGameObject(GameObject::Type type)
	{
		switch (type)
		{
		case GameObject::Type::QUAD:		
		{
			Ref<TS_ENGINE::Quad> quad = CreateRef<TS_ENGINE::Quad>("New Quad");
			//quad->SetMaterial(mDefaultMat);
			quad->Create();
			quad->GetNode()->AttachObject(quad);
			return quad;
		}
		break;

		case GameObject::Type::CUBE:
		{
			Ref<TS_ENGINE::Cube> cube = CreateRef<TS_ENGINE::Cube>("New Cube");
			//cube->SetMaterial(mDefaultMat);
			cube->Create();
			cube->GetNode()->AttachObject(cube);
			return cube;
		}
		break;

		case GameObject::Type::SPHERE:
		{
			Ref<TS_ENGINE::Sphere> sphere = CreateRef<TS_ENGINE::Sphere>("New Sphere");
			//sphere->SetMaterial(mDefaultMat);
			sphere->Create();
			sphere->GetNode()->AttachObject(sphere);
			return sphere;
		}
		break;

		case GameObject::Type::CONE:
		{
			//Ref<TS_ENGINE::Cone> cone = CreateRef<TS_ENGINE::Cone>("New Cone");
			//cone->SetMaterial(mDefaultMat);
			//cone->Create();
			//cone->GetNode()->AttachObject(cone);
			//return cone;
		}
		break;

		case GameObject::Type::CYLINDER:
		{
			//Ref<TS_ENGINE::Cylinder> cylinder = CreateRef<TS_ENGINE::Cylinder>("New Cylinder");
			//cylinder->SetMaterial(mDefaultMat);
			//cylinder->Create();
			//cylinder->GetNode()->AttachObject(cylinder);
			//return cylinder;
		}
		break;
		}

		return nullptr;
	}

#ifdef TS_ENGINE_EDITOR
	Ref<SceneCamera> Factory::CreateSceneCamera(Ref<Camera> editorCamera)
	{
		Ref<TS_ENGINE::SceneCamera> sceneCamera = CreateRef<TS_ENGINE::SceneCamera>("New SceneCamera", editorCamera);		
		return sceneCamera;		
	}
#else
	Ref<SceneCamera> Factory::CreateSceneCamera()
	{
		Ref<TS_ENGINE::SceneCamera> sceneCamera = CreateRef<TS_ENGINE::SceneCamera>("New SceneCamera");
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
