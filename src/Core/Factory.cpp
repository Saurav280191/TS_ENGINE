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

	Ref<Node> Factory::InstantitateEmptyNode(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> emptyNode = CreateRef<Node>();
		emptyNode->SetNodeRef(emptyNode);
		emptyNode->SetParent(parentNode);
		emptyNode->Initialize(name, EntityType::EMPTY);

		return emptyNode;
	}

	Ref<Node> Factory::InstantitateSceneCamera(const std::string& name
#ifdef TS_ENGINE_EDITOR
		, Ref<EditorCamera> editorCamera
#endif
	)
	{
		Ref<SceneCamera> sceneCamera = CreateRef<SceneCamera>("SceneCamera"
#ifdef TS_ENGINE_EDITOR
			,editorCamera
#endif
		);

		sceneCamera->SetPerspective(Camera::Perspective(60.0f, 1.77f, 0.5f, 50.0f));
		sceneCamera->CreateFramebuffer(800, 600);
		sceneCamera->Initialize();
		sceneCamera->GetNode()->SetSceneCamera(sceneCamera);
		sceneCamera->GetNode()->Initialize(name, EntityType::CAMERA);
		sceneCamera->GetNode()->SetNodeRef(sceneCamera->GetNode());

		return sceneCamera->GetNode();
	}

	Ref<Node> Factory::InstantitateDuplicateSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		Scene* scene = SceneManager::GetInstance()->GetCurrentScene().get();

#ifdef TS_ENGINE_EDITOR
		Ref<SceneCamera> duplicateSceneCamera = CreateRef<SceneCamera>("SceneCamera", scene->GetEditorCamera());
#else
		Ref<SceneCamera> duplicateSceneCamera = CreateRef<SceneCamera>("SceneCamera");
#endif
		if(sceneCamera->GetProjectionType() == SceneCamera::ProjectionType::PERSPECTIVE)
			duplicateSceneCamera->SetPerspective(sceneCamera->GetPerspective());
		else if(sceneCamera->GetProjectionType() == SceneCamera::ProjectionType::ORTHOGRAPHIC)
			duplicateSceneCamera->SetOrthographic(sceneCamera->GetOrthographic().top, sceneCamera->GetOrthographic().zNear, sceneCamera->GetOrthographic().zFar);

		//duplicateSceneCamera->SetPerspective(sceneCamera->GetPerspective());
		duplicateSceneCamera->CreateFramebuffer(sceneCamera->GetFramebuffer()->GetSpecification().Width, sceneCamera->GetFramebuffer()->GetSpecification().Height);
		duplicateSceneCamera->Initialize();
		duplicateSceneCamera->GetNode()->SetSceneCamera(duplicateSceneCamera);
		duplicateSceneCamera->GetNode()->SetParent(sceneCamera->GetNode()->GetParentNode());
		duplicateSceneCamera->GetNode()->Initialize(sceneCamera->GetNode()->GetEntity()->GetName() + " - Copy", EntityType::CAMERA);

		duplicateSceneCamera->GetNode()->GetTransform()->SetLocalPosition(sceneCamera->GetNode()->GetTransform()->GetLocalPosition());
		duplicateSceneCamera->GetNode()->GetTransform()->SetLocalRotation(sceneCamera->GetNode()->GetTransform()->GetLocalRotation());
		duplicateSceneCamera->GetNode()->GetTransform()->SetLocalScale(sceneCamera->GetNode()->GetTransform()->GetLocalScale());
		duplicateSceneCamera->GetNode()->ComputeTransformMatrices();

		scene->AddSceneCamera(duplicateSceneCamera);
		return duplicateSceneCamera->GetNode();
	}

	Ref<Node> Factory::InstantiateLine(const std::string& name, Ref<Node> parentNode, const std::vector<Vector3>& points)
	{
		// Create line mesh
		Ref<Line> line = CreateRef<Line>();
		line->CreateMesh(points);
		Ref<Mesh> mesh = line->GetMesh();
		mesh->SetName(name);
		
		// Create Node for line mesh
		Ref<Node> lineNode = CreateRef<Node>();
		lineNode->SetNodeRef(lineNode);
		lineNode->AddMesh(mesh);
		lineNode->SetParent(parentNode);
		lineNode->Initialize(name, EntityType::PRIMITIVE);
	
		return lineNode;
	}

	Ref<Node> Factory::InstantiateQuad(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> quadNode = CreateRef<Node>();
		quadNode->SetNodeRef(quadNode);
		Ref<Mesh> mesh = CreateRef<Quad>()->GetMesh();
		mesh->SetName(name);
		quadNode->AddMesh(mesh);
		quadNode->SetParent(parentNode);
		quadNode->Initialize(name, EntityType::PRIMITIVE);
		return quadNode;
	}

	Ref<Node> Factory::InstantiateCube(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> cubeNode = CreateRef<Node>();
		cubeNode->SetNodeRef(cubeNode);
		Ref<Mesh> mesh = CreateRef<Cube>()->GetMesh();
		mesh->SetName(name);
		cubeNode->AddMesh(mesh);
		cubeNode->SetParent(parentNode);
		cubeNode->Initialize(name, EntityType::PRIMITIVE);
		return cubeNode;
	}

	Ref<Node> Factory::InstantiateSphere(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> sphereNode = CreateRef<Node>();
		sphereNode->SetNodeRef(sphereNode);
		Ref<Mesh> mesh = CreateRef<Sphere>()->GetMesh();
		mesh->SetName(name);
		sphereNode->AddMesh(mesh);
		sphereNode->SetParent(parentNode);
		sphereNode->Initialize(name, EntityType::PRIMITIVE);
		return sphereNode;
	}

	Ref<Node> Factory::InstantiateCylinder(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> cylinderNode = CreateRef<Node>();
		cylinderNode->SetNodeRef(cylinderNode);
		Ref<Mesh> mesh = CreateRef<Cylinder>()->GetMesh();
		mesh->SetName(name);
		cylinderNode->AddMesh(mesh);
		cylinderNode->SetParent(parentNode);
		cylinderNode->Initialize(name, EntityType::PRIMITIVE);
		return cylinderNode;
	}

	Ref<Node> Factory::InstantiateCone(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> coneNode = CreateRef<Node>();
		coneNode->SetNodeRef(coneNode);
		Ref<Mesh> mesh = CreateRef<Cone>()->GetMesh();
		mesh->SetName(name);
		coneNode->AddMesh(mesh);
		coneNode->SetParent(parentNode);
		coneNode->Initialize(name, EntityType::PRIMITIVE);
		return coneNode;
	}

	Ref<Node> Factory::InstantiateModel(const std::string& modelPath, Ref<Node> parentNode)
	{
		Ref<Node> mModelNode = nullptr;
		Ref<Model> model = nullptr;

		auto it = mLoadedModelNodeMap.find(modelPath);

		if (it != mLoadedModelNodeMap.end())
		{
			mModelNode = it->second.first->Duplicate();
			mModelNode->GetTransform()->Reset();
		}
		else
		{
			model = ModelLoader::GetInstance()->LoadModel(modelPath);
			mModelNode = model->GetRootNode();
			
			mLoadedModelNodeMap.insert(std::pair<std::string, std::pair<Ref<Node>, Ref<Model>>>(modelPath, { mModelNode, model }));
		}

		mModelNode->SetModelPath(modelPath);
		mModelNode->SetParent(parentNode);

		return mModelNode;
	}

	void Factory::ChangeMeshForNode(Ref<Node> node, PrimitiveType primitive)
	{
		Ref<Material> firstMeshMaterial = node->GetMeshes()[0]->GetMaterial();
		node->RemoveAllMeshes();
		Ref<Mesh> mesh = nullptr;

		switch (primitive)
		{
		case PrimitiveType::QUAD:
			mesh = CreateRef<Quad>()->GetMesh();
			mesh->SetName("Quad");
			break;
		case PrimitiveType::CUBE:
			mesh = CreateRef<Cube>()->GetMesh();
			mesh->SetName("Cube");
			break;
		case PrimitiveType::SPHERE:
			mesh = CreateRef<Sphere>()->GetMesh();
			mesh->SetName("Sphere");
			break;
		case PrimitiveType::CYLINDER:
			mesh = CreateRef<Cylinder>()->GetMesh();
			mesh->SetName("Cylinder");
			break;
		case PrimitiveType::CONE:
			mesh = CreateRef<Cone>()->GetMesh();
			mesh->SetName("Cone");
			break;
		}

		mesh->SetMaterial(firstMeshMaterial);
		node->AddMesh(mesh);
	}

	void Factory::Flush()
	{
		mLoadedModelNodeMap.clear();
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
