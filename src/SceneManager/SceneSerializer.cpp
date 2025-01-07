#include "tspch.h"
#include "SceneManager/Scene.h"
#include "SceneManager/SceneSerializer.h"
//#include <Factory.h>
#include "Renderer/Texture.h"

namespace TS_ENGINE
{
	SceneSerializer::SceneSerializer()
	{
		
	}

	SceneSerializer::~SceneSerializer()
	{

	}

	void SceneSerializer::Save(Ref<Scene> scene)
	{
		nlohmann::json json;
		std::string sceneName = scene->GetSceneNode()->GetEntity()->GetName();

		json["Scene"]["Name"] = sceneName;

#ifdef TS_ENGINE_EDITOR
		// Editor Camera
		{
			//Transform
			json["Scene"]["EditorCamera"]["Transform"] = {
				{ "LocalPosition", {scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalPosition().x, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalPosition().y, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalPosition().z}},
				{ "LocalEulerAngles", {scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalEulerAngles().x, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalEulerAngles().y, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalEulerAngles().z}},
				{ "LocalScale", {scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalScale().x, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalScale().y, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalScale().z}}
			};

			//Projection
			if (scene->GetEditorCamera()->GetProjectionType() == Camera::ProjectionType::PERSPECTIVE)// Perspective 
			{
				json["Scene"]["EditorCamera"]["ProjectionType"] = Camera::ProjectionType::PERSPECTIVE,

					json["Scene"]["EditorCamera"]["Projection"] =
				{
					{"fov", scene->GetEditorCamera()->GetPerspective().fov},
					{"aspectRatio", scene->GetEditorCamera()->GetPerspective().aspectRatio},
					{"zNear", scene->GetEditorCamera()->GetPerspective().zNear},
					{"zFar", scene->GetEditorCamera()->GetPerspective().zFar},
				};
			}
			else if (scene->GetEditorCamera()->GetProjectionType() == Camera::ProjectionType::ORTHOGRAPHIC) // Orthographic
			{
				json["Scene"]["EditorCamera"]["ProjectionType"] = Camera::ProjectionType::ORTHOGRAPHIC,

					json["Scene"]["EditorCamera"]["Projection"] =
				{
					{"size", scene->GetEditorCamera()->GetOrthographic().top},
					{"zNear", scene->GetEditorCamera()->GetOrthographic().zNear},
					{"zFar", scene->GetEditorCamera()->GetOrthographic().zFar},
				};
			}
		}
#endif

		// Scene cameras
		{
			for (int i = 0; i < scene->GetSceneCameras().size(); i++)
			{
				// Name
				json["Scene"]["SceneCameras"][i]["Name"] = scene->GetSceneCameras()[i]->GetNode()->GetEntity()->GetName();

				// Transform
				json["Scene"]["SceneCameras"][i]["Transform"] =
				{
					{ "LocalPosition", { scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalPosition().x, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalPosition().y, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalPosition().z} },
					{ "LocalEulerAngles", { scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalEulerAngles().x, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalEulerAngles().y, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalEulerAngles().z} },
					{ "LocalScale", { scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalScale().x, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalScale().y, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalScale().z} },
				};

				// Projection
				if (scene->GetSceneCameras()[i]->GetProjectionType() == Camera::ProjectionType::PERSPECTIVE)// Perspective 
				{
					json["Scene"]["SceneCameras"][i]["ProjectionType"] = Camera::ProjectionType::PERSPECTIVE,

						json["Scene"]["SceneCameras"][i]["Projection"] = {
							{"fov", scene->GetSceneCameras()[i]->GetPerspective().fov},
							{"aspectRatio", scene->GetSceneCameras()[i]->GetPerspective().aspectRatio},
							{"zNear", scene->GetSceneCameras()[i]->GetPerspective().zNear},
							{"zFar", scene->GetSceneCameras()[i]->GetPerspective().zFar},
					};
				}
				else if (scene->GetSceneCameras()[i]->GetProjectionType() == Camera::ProjectionType::ORTHOGRAPHIC) // Orthographic
				{
					json["Scene"]["SceneCameras"][i]["ProjectionType"] = Camera::ProjectionType::ORTHOGRAPHIC,

						json["Scene"]["SceneCameras"][i]["Projection"] = {
							{"size", scene->GetSceneCameras()[i]->GetOrthographic().top},
							{"zNear", scene->GetSceneCameras()[i]->GetOrthographic().zNear},
							{"zFar", scene->GetSceneCameras()[i]->GetOrthographic().zFar},
					};
				}
			}
		}

		// Nodes (Recursively iterate through scene node children and serialize them)	
		json["Scene"]["SceneData"] = SerializeNode(scene->GetSceneNode());

		std::string filePath = Application::s_SaveSceneDir.string() + "\\" + sceneName + ".scene";
		std::ofstream o(filePath);
		o << std::setw(4) << json << std::endl;

		//auto framebufferImage = scene->GetEditorCamera()->GetFramebuffer()->GetFrameBufferImage(0, 219);
		//WritePixelsToFile(framebufferImage, "..\\..\\..\\Resources\\SavedSceneThumbnails\\" + std::string(sceneName) + ".png");
		TS_CORE_INFO("*** Scene saved ***");
	}

	void SceneSerializer::Load(const std::string& savedScenePath)
	{
		TS_CORE_TRACE("Loading: {0}", savedScenePath);
		nlohmann::json jsonData = GetJsonDataFromFile(savedScenePath);

		// Scene Name
		std::string sceneName = jsonData["Scene"]["Name"];

		// Editor Camera
		Camera::ProjectionType projectionType = jsonData["Scene"]["EditorCamera"]["ProjectionType"];

		Ref<EditorCamera> editorCamera = CreateRef<EditorCamera>("EditorCamera");

		// Editor Camera Perspective
		if (projectionType == Camera::ProjectionType::PERSPECTIVE)
		{
			float fov = jsonData["Scene"]["EditorCamera"]["Projection"]["fov"];
			float aspectRatio = jsonData["Scene"]["EditorCamera"]["Projection"]["aspectRatio"];
			float zNear = jsonData["Scene"]["EditorCamera"]["Projection"]["zNear"];
			float zFar = jsonData["Scene"]["EditorCamera"]["Projection"]["zFar"];

			editorCamera->SetPerspective(fov, aspectRatio, zNear, zFar);
		}
		else if (projectionType == Camera::ProjectionType::ORTHOGRAPHIC)
		{
			float size = jsonData["Scene"]["EditorCamera"]["Projection"]["size"];
			float zNear = jsonData["Scene"]["EditorCamera"]["Projection"]["zNear"];
			float zFar = jsonData["Scene"]["EditorCamera"]["Projection"]["zFar"];

			editorCamera->SetOrthographic(size, zNear, zFar);
		}

		// Editor Camera Transform
		Vector3 localPosition = Vector3(jsonData["Scene"]["EditorCamera"]["Transform"]["LocalPosition"][0],
			jsonData["Scene"]["EditorCamera"]["Transform"]["LocalPosition"][1], jsonData["Scene"]["EditorCamera"]["Transform"]["LocalPosition"][2]);

		Vector3 localEulerAngles = Vector3(jsonData["Scene"]["EditorCamera"]["Transform"]["LocalEulerAngles"][0],
			jsonData["Scene"]["EditorCamera"]["Transform"]["LocalEulerAngles"][1], jsonData["Scene"]["EditorCamera"]["Transform"]["LocalEulerAngles"][2]);

		editorCamera->GetNode()->GetTransform()->SetLocalPosition(localPosition);
		editorCamera->GetNode()->GetTransform()->SetLocalEulerAngles(localEulerAngles);
		editorCamera->CreateFramebuffer(1920, 1080);//Create framebuffer for editorCamera
		editorCamera->Initialize();
		editorCamera->GetNode()->ReInitializeTransforms();

		// Scene Cameras
		std::vector<Ref<SceneCamera>> sceneCameras = {};

		for (int i = 0; i < jsonData["Scene"]["SceneCameras"].size(); i++)
		{
			// Scene Camera Name
			std::string name = jsonData["Scene"]["SceneCameras"][i]["Name"];

			Ref<SceneCamera> sceneCamera = nullptr;
			
#ifdef TS_ENGINE_EDITOR
			sceneCamera = Factory::GetInstance()->InstantitateSceneCamera(name, editorCamera)->GetSceneCamera();
#else
			sceneCamera = Factory::GetInstance()->InstantitateSceneCamera(name)->GetSceneCamera();
#endif

			Camera::ProjectionType projectionType = jsonData["Scene"]["SceneCameras"][i]["ProjectionType"];

			// Scene Camera Perspective
			if (projectionType == Camera::ProjectionType::PERSPECTIVE)
			{
				float fov = jsonData["Scene"]["SceneCameras"][i]["Projection"]["fov"];
				float aspectRatio = jsonData["Scene"]["SceneCameras"][i]["Projection"]["aspectRatio"];
				float zNear = jsonData["Scene"]["SceneCameras"][i]["Projection"]["zNear"];
				float zFar = jsonData["Scene"]["SceneCameras"][i]["Projection"]["zFar"];

				sceneCamera->SetPerspective(fov, aspectRatio, zNear, zFar);
			}
			else if (projectionType == Camera::ProjectionType::ORTHOGRAPHIC)
			{
				float size = jsonData["Scene"]["SceneCameras"][i]["Projection"]["size"];
				float zNear = jsonData["Scene"]["SceneCameras"][i]["Projection"]["zNear"];
				float zFar = jsonData["Scene"]["SceneCameras"][i]["Projection"]["zFar"];

				sceneCamera->SetOrthographic(size, zNear, zFar);
			}

			// Scene Camera Transform
			Vector3 localPosition = Vector3(jsonData["Scene"]["SceneCameras"][i]["Transform"]["LocalPosition"][0],
				jsonData["Scene"]["SceneCameras"][i]["Transform"]["LocalPosition"][1], jsonData["Scene"]["SceneCameras"][i]["Transform"]["LocalPosition"][2]);

			Vector3 localEulerAngles = Vector3(jsonData["Scene"]["SceneCameras"][i]["Transform"]["LocalEulerAngles"][0],
				jsonData["Scene"]["SceneCameras"][i]["Transform"]["LocalEulerAngles"][1], jsonData["Scene"]["SceneCameras"][i]["Transform"]["LocalEulerAngles"][2]);

			sceneCamera->GetNode()->GetTransform()->SetLocalPosition(localPosition);
			sceneCamera->GetNode()->GetTransform()->SetLocalEulerAngles(localEulerAngles);
			sceneCamera->Initialize();
			//sceneCamera->GetNode()->InitializeTransformMatrices();

			sceneCameras.push_back(sceneCamera);
		}

		Ref<Scene> scene = CreateRef<Scene>(sceneName);					// Create new scene
		scene->AddEditorCamera(editorCamera);							// Add editor camera to scene

		for (auto& sceneCamera : sceneCameras)							// Add scene cameras to scene
		{
			scene->AddSceneCamera(sceneCamera);
			sceneCamera->GetNode()->SetParent(scene->GetSceneNode());
		}

		// Iterate through all the children nodes for root node
		{
			size_t numRootChildren = jsonData["Scene"]["SceneData"]["Children"].size();
			
			for (size_t i = 0; i < numRootChildren; i++)
			{
				nlohmann::json jsonNode = jsonData["Scene"]["SceneData"]["Children"][i];
				DeserializeNode(scene->GetSceneNode(), jsonNode, editorCamera);
			}
		}

		SceneManager::GetInstance()->SetCurrentScene(scene);
	}

	nlohmann::json SceneSerializer::SerializeNode(Ref<Node> node)
	{
		nlohmann::json jsonNode;

		jsonNode["Name"] = node->GetEntity()->GetName().c_str();

#ifdef TS_ENGINE_EDITOR
		jsonNode["Enabled"] = node->m_Enabled;
#endif
		
		jsonNode["EntityType"] = node->GetEntity()->GetEntityType();

		// EntityType And PrimitiveType
		if (node->GetEntity()->GetEntityType() == EntityType::PRIMITIVE)
		{
			jsonNode["PrimitiveType"] = node->GetMeshes()[0]->GetPrimitiveType();
		}
		else if (node->GetEntity()->GetEntityType() == EntityType::MODELROOTNODE)
		{
			if (node->GetModelPath() != "")
			{
				jsonNode["ModelPath"] = node->GetModelPath().c_str();
			}
		}

		// Mesh And Material
		if (node->GetEntity()->GetEntityType() == EntityType::PRIMITIVE || node->GetEntity()->GetEntityType() == EntityType::MESH)
		{
			for (int i = 0; i < node->GetMeshes().size(); i++)
			{
				std::string meshName = node->GetMeshes()[i]->GetName();
				Vector4 ambientColor = node->GetMeshes()[i]->GetMaterial()->GetAmbientColor();
				Vector4 diffuseColor = node->GetMeshes()[i]->GetMaterial()->GetDiffuseColor();
				Vector4 specularColor = node->GetMeshes()[i]->GetMaterial()->GetSpecularColor();

				jsonNode["Meshes"][i]["Name"] = meshName;

				jsonNode["Meshes"][i]["Material"]["Name"] = node->GetMeshes()[i]->GetMaterial()->GetName();
				jsonNode["Meshes"][i]["Material"]["Ambient"]["Color"] = { ambientColor.x, ambientColor.y, ambientColor.z, ambientColor.w };

				jsonNode["Meshes"][i]["Material"]["Diffuse"]["Color"] = { diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w };
				if (node->GetMeshes()[i]->GetMaterial()->GetDiffuseMap() && node->GetMeshes()[i]->GetMaterial()->GetDiffuseMap()->GetPath() != "")
				{
					jsonNode["Meshes"][i]["Material"]["Diffuse"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetDiffuseMap()->GetPath();
				}
				jsonNode["Meshes"][i]["Material"]["Diffuse"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapOffset().y };
				jsonNode["Meshes"][i]["Material"]["Diffuse"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapTiling().y };

				jsonNode["Meshes"][i]["Material"]["Specular"]["Color"] = { specularColor.x, specularColor.y, specularColor.z, specularColor.w };
				if (node->GetMeshes()[i]->GetMaterial()->GetSpecularMap() && node->GetMeshes()[i]->GetMaterial()->GetSpecularMap()->GetPath() != "")
				{
					jsonNode["Meshes"][i]["Material"]["Specular"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetSpecularMap()->GetPath();
				}
				jsonNode["Meshes"][i]["Material"]["Specular"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetSpecularMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetSpecularMapOffset().y };
				jsonNode["Meshes"][i]["Material"]["Specular"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetSpecularMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetSpecularMapTiling().y };
				jsonNode["Meshes"][i]["Material"]["Specular"]["Shininess"] = node->GetMeshes()[i]->GetMaterial()->GetShininess();

				if (node->GetMeshes()[i]->GetMaterial()->GetNormalMap() && node->GetMeshes()[i]->GetMaterial()->GetNormalMap()->GetPath() != "")
				{
					jsonNode["Meshes"][i]["Material"]["Normal"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetNormalMap()->GetPath();
				}
				jsonNode["Meshes"][i]["Material"]["Normal"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetNormalMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetNormalMapOffset().y };
				jsonNode["Meshes"][i]["Material"]["Normal"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetNormalMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetNormalMapTiling().y };
				jsonNode["Meshes"][i]["Material"]["Normal"]["BumpValue"] = node->GetMeshes()[i]->GetMaterial()->GetBumpValue();
			}
		}

		// Transform
		jsonNode["Transform"] =
		{
			{"LocalPosition", {node->GetTransform()->GetLocalPosition().x, node->GetTransform()->GetLocalPosition().y, node->GetTransform()->GetLocalPosition().z}},
			{"LocalEulerAngles", {node->GetTransform()->GetLocalEulerAngles().x, node->GetTransform()->GetLocalEulerAngles().y, node->GetTransform()->GetLocalEulerAngles().z}},
			{"LocalScale", {node->GetTransform()->GetLocalScale().x, node->GetTransform()->GetLocalScale().y, node->GetTransform()->GetLocalScale().z}}
		};

		// Children
		nlohmann::json childrenJson = nlohmann::json::array();
		for (Ref<Node> childNode : node->GetChildren())
		{
			childrenJson.push_back(SerializeNode(childNode));
		}
		
		if(node->GetChildren().size() > 0)
			jsonNode["Children"] = childrenJson;

		TS_CORE_INFO(node->GetEntity()->GetName() + " node serialized!");
		
		return jsonNode;
	}

	nlohmann::json SceneSerializer::GetJsonDataFromFile(const std::string& filePath)
	{
		try {
			// Open the JSON file for reading
			std::ifstream file(filePath);

			// Check if the file is open
			if (!file.is_open()) {
				TS_CORE_ERROR("Error: Failed to open the JSON file!");
			}

			// Read the JSON content from the file into a JSON object
			nlohmann::json jsonData;
			file >> jsonData;

			// Close the file
			file.close();

			return jsonData;
		}
		catch (const std::exception& e) {
			TS_CORE_ERROR("Error: {0}", e.what());
		}

		return nullptr;
	}

	void SceneSerializer::DeserializeNode(Ref<Node> parentNode, nlohmann::json& jsonNode, Ref<EditorCamera> editorCamera)
	{
		std::string name = jsonNode["Name"];
		EntityType entityType = (EntityType)jsonNode["EntityType"];

		if (name == "Ground")
		{
			TS_CORE_INFO("DeserializeNode called -> Instantiating ground!");
		}

		if (entityType == EntityType::PRIMITIVE)
		{
			PrimitiveType primitiveType = (PrimitiveType)jsonNode["PrimitiveType"];

			switch (primitiveType)
			{
			case PrimitiveType::QUAD:
			{
				Ref<Node> node = Factory::GetInstance()->InstantiateQuad(name, parentNode);
				ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);
			}
			break;
			case PrimitiveType::CUBE:
			{
				Ref<Node> node = Factory::GetInstance()->InstantiateCube(name, parentNode);
				ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);
			}
			break;
			case PrimitiveType::SPHERE:
			{
				Ref<Node> node = Factory::GetInstance()->InstantiateSphere(name, parentNode);
				ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);
			}
			break;
			case PrimitiveType::CYLINDER:
			{
				Ref<Node> node = Factory::GetInstance()->InstantiateCylinder(name, parentNode);
				ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);
			}
			break;
			case PrimitiveType::CONE:
			{
				Ref<Node> node = Factory::GetInstance()->InstantiateCone(name, parentNode);				
				ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);
			}
			break;
			}
		}
		else if (entityType == EntityType::MODELROOTNODE)
		{
			auto it = jsonNode.find("ModelPath");

			if (it != jsonNode.end())// For Model's RootNode
			{
				// Load model
				Ref<Node> modelRootNode = Factory::GetInstance()->InstantiateModel(jsonNode["ModelPath"], parentNode);				
				
				// Apply saved transform to nodes
				ApplyAndDeserializeChildrenNode(modelRootNode, jsonNode, editorCamera);
				
				// Deserialization not needed for Model root childern children since it is taken care of while Instantiating Model
				/*for (int i = 0; i < modelRootNode->GetChildCount(); i++)
				{
					nlohmann::json childJsonNode = jsonNode["Children"][i];
					DeserializeModelNode(modelRootNode->GetChildAt(i), modelRootNode, childJsonNode, editorCamera);
				}*/				
			}
		}
		else if (entityType == EntityType::BONE)
		{

		}
		else if (entityType == EntityType::BONEGUI)
		{

		}
		else if (entityType == EntityType::MESH)
		{
			// TODO: Can be extended later to save models in custom encrypted model format
		}
		else if (entityType == EntityType::CAMERA)
		{
			Ref<Node> sceneCameraNode = nullptr;
#ifdef TS_ENGINE_EDITOR
			sceneCameraNode = Factory::GetInstance()->InstantitateSceneCamera(name, editorCamera);// No need to set parent for camera. It gets set during scene creation process			
#else
			sceneCameraNode = Factory::GetInstance()->InstantitateSceneCamera(name);// No need to set parent for camera. It gets set during scene creation process			
#endif
			ApplyAndDeserializeChildrenNode(sceneCameraNode, jsonNode, editorCamera);
			//mSceneCameraNodes.push_back(sceneCameraNode);
		}
		else if (entityType == EntityType::EMPTY)
		{
			Ref<Node> node = Factory::GetInstance()->InstantitateEmptyNode(name, parentNode);
			ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);
		}
		else
		{
			TS_CORE_ERROR("Attempted to load unknown node type");
			/*Ref<Node> node = CreateRef<Node>();
			node->SetNodeRef(node);
			parentNode->AddChild(node);
			ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);*/
		}
	}

	void SceneSerializer::DeserializeModelNode(Ref<Node> node, Ref<Node> parentNode, nlohmann::json& jsonNode, Ref<EditorCamera> editorCamera)
	{
		std::string name = "";
		if (jsonNode.contains("Name") && jsonNode["Name"] != NULL)
			name = jsonNode["Name"];
	
		TS_CORE_ASSERT(name != "");

		EntityType entityType = (EntityType)jsonNode["EntityType"];
		
		ApplyAndDeserializeChildrenNode(node, jsonNode, editorCamera);

		for (int i = 0; i < node->GetChildCount(); i++)
		{
			nlohmann::json childJsonNode = jsonNode["Children"][i];
			DeserializeModelNode(node, parentNode, childJsonNode, editorCamera);
		}
	}

	void SceneSerializer::ApplyAndDeserializeChildrenNode(Ref<Node> node, nlohmann::json& jsonNode, Ref<EditorCamera> editorCamera)
	{
		if (node != nullptr)
		{
			std::string name = jsonNode["Name"];
			EntityType entityType = (EntityType)jsonNode["EntityType"];

			if (name == "Ground")
			{
				TS_CORE_INFO("ApplyAndDeserializeChildrenNode called -> Instantiating ground!");
			}

			ApplyTransformAndMaterialForNode(node, jsonNode);

			//node->Initialize(name, entityType);
			node->ReInitializeTransforms();

			// Deserialize children
			if (jsonNode["Children"] != nullptr)						// Making sure Children json node exists 
			{
				for (int i = 0; i < jsonNode["Children"].size(); i++)	// Iterate through all the children
				{
					nlohmann::json childJsonNode = jsonNode["Children"][i];
					DeserializeNode(node, childJsonNode, editorCamera);
				}
			}
		}
	}

	void SceneSerializer::ApplyTransformAndMaterialForNode(Ref<Node> node, nlohmann::json& jsonNode)
	{
		// Apply Transforms
		Vector3 transformPosition = Vector3(
			jsonNode["Transform"]["LocalPosition"][0],
			jsonNode["Transform"]["LocalPosition"][1],
			jsonNode["Transform"]["LocalPosition"][2]);

		Vector3 transformEulerAngles = Vector3(
			jsonNode["Transform"]["LocalEulerAngles"][0],
			jsonNode["Transform"]["LocalEulerAngles"][1],
			jsonNode["Transform"]["LocalEulerAngles"][2]);

		Vector3 transformScale = Vector3(
			jsonNode["Transform"]["LocalScale"][0],
			jsonNode["Transform"]["LocalScale"][1],
			jsonNode["Transform"]["LocalScale"][2]);

		// Set local transforms
		node->GetTransform()->SetLocalTransform(transformPosition, transformEulerAngles, transformScale, node->GetParentNode());

		// Apply Material
		for (int i = 0; i < jsonNode["Meshes"].size(); i++)
		{
			nlohmann::json material = jsonNode["Meshes"][i]["Material"];

			// Colors
			Vector4 ambientColor = Vector4(
				material["Ambient"]["Color"][0],
				material["Ambient"]["Color"][1],
				material["Ambient"]["Color"][2],
				material["Ambient"]["Color"][3]
			);
			Vector4 diffuseColor = Vector4(
				material["Diffuse"]["Color"][0],
				material["Diffuse"]["Color"][1],
				material["Diffuse"]["Color"][2],
				material["Diffuse"]["Color"][3]
			);
			Vector4 specularColor = Vector4(
				material["Specular"]["Color"][0],
				material["Specular"]["Color"][1],
				material["Specular"]["Color"][2],
				material["Specular"]["Color"][3]
			);

			// Textures

			Ref<Texture2D> diffuseTexture = nullptr;
			{
				auto it = material["Diffuse"].find("TexturePath");

				if (it != material["Diffuse"].end())
				{
					diffuseTexture = Texture2D::Create(material["Diffuse"]["TexturePath"]);
				}
			}

			Ref<Texture2D> specularTexture = nullptr;
			{
				auto it = material["Specular"].find("TexturePath");

				if (it != material["Specular"].end())
				{
					specularTexture = Texture2D::Create(material["Specular"]["TexturePath"]);
				}
			}

			Ref<Texture2D> normalTexture = nullptr;
			{
				auto it = material["Normal"].find("TexturePath");

				if (it != material["Normal"].end())
				{
					normalTexture = Texture2D::Create(material["Normal"]["TexturePath"]);
				}
			}

			// Offset And Tiling
			Vector2 diffuseTextureOffset = Vector2(material["Diffuse"]["TextureOffset"][0], material["Diffuse"]["TextureOffset"][1]);
			Vector2 diffuseTextureTiling = Vector2(material["Diffuse"]["TextureTiling"][0], material["Diffuse"]["TextureTiling"][1]);

			Vector2 specularTextureOffset = Vector2(material["Specular"]["TextureOffset"][0], material["Specular"]["TextureOffset"][1]);
			Vector2 specularTextureTiling = Vector2(material["Specular"]["TextureTiling"][0], material["Specular"]["TextureTiling"][1]);

			Vector2 normalTextureOffset = Vector2(material["Normal"]["TextureOffset"][0], material["Normal"]["TextureOffset"][1]);
			Vector2 normalTextureTiling = Vector2(material["Normal"]["TextureTiling"][0], material["Normal"]["TextureTiling"][1]);

			// Shininess And BumpValue
			float shininess = material["Specular"]["Shininess"];
			float bumpValue = material["Normal"]["BumpValue"];

			// Apply Material Properties
			node->GetMeshes()[i]->GetMaterial()->SetAmbientColor(ambientColor);
			node->GetMeshes()[i]->GetMaterial()->SetDiffuseColor(diffuseColor);
			node->GetMeshes()[i]->GetMaterial()->SetSpecularColor(specularColor);

			if(diffuseTexture)
				node->GetMeshes()[i]->GetMaterial()->SetDiffuseMap(diffuseTexture);
			if(specularTexture)
				node->GetMeshes()[i]->GetMaterial()->SetSpecularMap(specularTexture);
			if(normalTexture)
				node->GetMeshes()[i]->GetMaterial()->SetNormalMap(normalTexture);

			node->GetMeshes()[i]->GetMaterial()->SetDiffuseMapOffset(diffuseTextureOffset);
			node->GetMeshes()[i]->GetMaterial()->SetDiffuseMapTiling(diffuseTextureTiling);
			node->GetMeshes()[i]->GetMaterial()->SetSpecularMapOffset(specularTextureOffset);
			node->GetMeshes()[i]->GetMaterial()->SetSpecularMapTiling(specularTextureTiling);
			node->GetMeshes()[i]->GetMaterial()->SetNormalMapOffset(normalTextureOffset);
			node->GetMeshes()[i]->GetMaterial()->SetNormalMapTiling(normalTextureTiling);

			node->GetMeshes()[i]->GetMaterial()->SetShininess(shininess);
			node->GetMeshes()[i]->GetMaterial()->SetBumpValue(bumpValue);
		}
		
		// Apply Enabled
		bool enabled = jsonNode["Enabled"];
#ifdef TS_ENGINE_EDITOR
		node->m_Enabled = enabled;
#endif
	}
}
