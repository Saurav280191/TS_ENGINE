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
		emptyNode->SetParent(parentNode);
		emptyNode->Initialize(name, NodeType::EMPTY);

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
		sceneCamera->GetNode()->Initialize(name, NodeType::CAMERA);

		return sceneCamera->GetNode();
	}

	Ref<Node> Factory::InstantitateDuplicateSceneCamera(Ref<SceneCamera> sceneCamera)
	{
		Ref<Scene>& scene = SceneManager::GetInstance()->GetCurrentScene();

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
		duplicateSceneCamera->GetNode()->Initialize(sceneCamera->GetNode()->mName + " - Copy", NodeType::CAMERA);

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
		lineNode->AddMesh(mesh);
		lineNode->SetParent(parentNode);
		lineNode->Initialize(name, NodeType::PRIMITIVE);
	
		return lineNode;
	}

	Ref<Node> Factory::InstantiateQuad(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> quadNode = CreateRef<Node>();
		Ref<Quad> quad = CreateRef<Quad>();
		Ref<Mesh> mesh = quad->GetMesh();
		mesh->SetName(name);
		quadNode->AddMesh(mesh);
		quadNode->SetParent(parentNode);
		quadNode->Initialize(name, NodeType::PRIMITIVE);
		return quadNode;
	}

	Ref<Node> Factory::InstantiateCube(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> cubeNode = CreateRef<Node>();
		Ref<Mesh> mesh = CreateRef<Cube>()->GetMesh();
		mesh->SetName(name);
		cubeNode->AddMesh(mesh);
		cubeNode->SetParent(parentNode);
		cubeNode->Initialize(name, NodeType::PRIMITIVE);
		return cubeNode;
	}

	Ref<Node> Factory::InstantiateSphere(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> sphereNode = CreateRef<Node>();;
		Ref<Mesh> mesh = CreateRef<Sphere>()->GetMesh();
		mesh->SetName(name);
		sphereNode->AddMesh(mesh);
		sphereNode->SetParent(parentNode);
		sphereNode->Initialize(name, NodeType::PRIMITIVE);
		return sphereNode;
	}

	Ref<Node> Factory::InstantiateSphere(const std::string& _name, float _radius, Ref<Node> _parentNode)
	{
		Ref<Node> sphereNode = CreateRef<Node>();
		Ref<Mesh> mesh = CreateRef<Sphere>(_radius)->GetMesh();
		mesh->SetName(_name);
		sphereNode->AddMesh(mesh);
		sphereNode->SetParent(_parentNode);
		sphereNode->Initialize(_name, NodeType::PRIMITIVE);
		return sphereNode;
	}

	Ref<Node> Factory::InstantiateCylinder(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> cylinderNode = CreateRef<Node>();
		Ref<Mesh> mesh = CreateRef<Cylinder>()->GetMesh();
		mesh->SetName(name);
		cylinderNode->AddMesh(mesh);
		cylinderNode->SetParent(parentNode);
		cylinderNode->Initialize(name, NodeType::PRIMITIVE);
		return cylinderNode;
	}

	Ref<Node> Factory::InstantiateCone(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> coneNode = CreateRef<Node>();
		Ref<Mesh> mesh = CreateRef<Cone>()->GetMesh();
		mesh->SetName(name);
		coneNode->AddMesh(mesh);
		coneNode->SetParent(parentNode);
		coneNode->Initialize(name, NodeType::PRIMITIVE);
		return coneNode;
	}

	Ref<Node> Factory::InstantiateSphereGui(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> sphereNode = CreateRef<Node>();		
		Ref<Mesh> mesh = CreateRef<Sphere>()->GetMesh();
		mesh->SetName(name);
		sphereNode->AddMesh(mesh);
		sphereNode->SetParent(parentNode);
		sphereNode->Initialize(name, NodeType::BONEGUI);
		return sphereNode;
	}

	Ref<Node> Factory::InstantiateBone(const std::string& name, Ref<Node> parentNode)
	{
		Ref<Node> boneNode = CreateRef<Node>();		
		Ref<Mesh> mesh = CreateRef<Mesh>();

		mesh->AddVertex(Vertex(Vector3(-0.5f, 0.0f, 0.0f)));// Left Tip

		mesh->AddVertex(Vertex(Vector3(0.3f, -0.5f, -0.5f)));	// Bottom-left
		mesh->AddVertex(Vertex(Vector3(0.3f, -0.5f, 0.5f)));	// Bottom-right
		mesh->AddVertex(Vertex(Vector3(0.3f, 0.5f, 0.5f)));	// Top-right
		mesh->AddVertex(Vertex(Vector3(0.3f, 0.5f, -0.5f)));	// Top-left

		mesh->AddVertex(Vertex(Vector3(0.5f, 0.0f, 0.0f)));	// Right Tip

		// Left pyramid
		mesh->AddIndex(0);
		mesh->AddIndex(1);
		mesh->AddIndex(2);

		mesh->AddIndex(0);
		mesh->AddIndex(2);
		mesh->AddIndex(3);

		mesh->AddIndex(0);
		mesh->AddIndex(3);
		mesh->AddIndex(4);

		mesh->AddIndex(0);
		mesh->AddIndex(4);
		mesh->AddIndex(1);

		// Right pyramid
		mesh->AddIndex(5);
		mesh->AddIndex(1);
		mesh->AddIndex(2);

		mesh->AddIndex(5);
		mesh->AddIndex(2);
		mesh->AddIndex(3);

		mesh->AddIndex(5);
		mesh->AddIndex(3);
		mesh->AddIndex(4);

		mesh->AddIndex(5);
		mesh->AddIndex(4);
		mesh->AddIndex(1);

		mesh->Create(DrawMode::TRIANGLE);

		mesh->SetName(name);
		boneNode->AddMesh(mesh);
		boneNode->SetParent(parentNode);
		boneNode->Initialize(name, NodeType::BONEGUI);
		return boneNode;
	}

	std::pair<Ref<Node>, Ref<Model>> Factory::InstantiateModel(const std::string& modelPath, Ref<Node> parentNode)
	{
		Ref<Node> modelRootNode = nullptr;
		Ref<Model> model = nullptr;

		auto it = mLoadedModelNodeMap.find(modelPath);

		// If model is found in map
		if (it != mLoadedModelNodeMap.end())
		{
			auto&[originalRootNode, originalModel] = it->second;
			
			std::unordered_map<std::string, Ref<Bone>> boneInfoMap = {};

			for (auto& [originalBoneName, originalBone] : originalModel->GetBoneInfoMap())
			{
				if (originalBone)
				{
					Ref<Bone> bone = CreateRef<Bone>(*originalBone);
					boneInfoMap.insert({ originalBoneName, bone });
				}
			}

			modelRootNode = originalRootNode->Duplicate(); 
			
			for (auto& child : modelRootNode->GetChildren())
			{
				child->SetModelRootNodeId(modelRootNode->mId);
			}

			// SetNodesForBones
			for (auto& [name, bone] : boneInfoMap)
			{
				if (bone)
				{
					Ref<Node> node = modelRootNode->FindNodeByName(name);
					bone->SetNode(node);
					node->SetBone(bone);
				}
			}
			
			// CreateBoneGuis
			for (auto& [name, bone] : boneInfoMap)
			{
				if (bone)
				{
					bone->CreateGui();
				}
			}

			// Duplicate animation
			for (auto& [originalAnimationName, originalAnimation] : originalRootNode->GetAnimations())
			{
				Ref<Animation> duplicateAnimation = CreateRef<Animation>(*originalAnimation);
				duplicateAnimation->mInitializedNodesForAnimation = false;
				modelRootNode->AddAnimation(duplicateAnimation);
			}
			modelRootNode->SetCurrentAnimation(0);
		}
		else// New model
		{
			model = ModelLoader::GetInstance()->LoadModel(modelPath);
			modelRootNode = model->GetRootNode();
			
			mLoadedModelNodeMap.insert(std::pair<std::string, std::pair<Ref<Node>, Ref<Model>>>(modelPath, { modelRootNode, model }));
		}

		modelRootNode->SetModelPath(modelPath);
		//modelRootNode->SetParent(parentNode);
		parentNode->AddChild(modelRootNode);

		return { modelRootNode, model };
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

	void Factory::Clear()
	{
		mLoadedModelNodeMap.clear();
	}

	const Ref<Node> Factory::FindNodeByName(const std::string& _name)
	{
		for (auto& loadeModelNodePair : mLoadedModelNodeMap)
		{
			Ref<Model> model = loadeModelNodePair.second.second;
			Ref<Node> foundNode = model->FindNodeByName(_name);
		
			if (foundNode)
				return foundNode;
			else
				return nullptr;
		}

		return nullptr;
	}

	int Factory::GetBoneIdByName(std::string& _name)
	{
		for (auto& loadeModelNodePair : mLoadedModelNodeMap)
		{
			Ref<Model> model = loadeModelNodePair.second.second;
			Ref<Bone> foundBone = model->FindBoneByName(_name);

			if (foundBone)
				return foundBone->GetId();
			else
				return -1;
		}

		return -1;// -1 is the no bone selected default value
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
