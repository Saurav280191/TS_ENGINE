#include "tspch.h"
#include "SceneManager/Scene.h"
#include "SceneManager/SceneSerializer.h"
//#include <Factory.h>
#include "Renderer/Texture.h"

namespace TS_ENGINE
{
	void SceneSerializer::Save(Ref<Scene> scene)
	{
		nlohmann::json json;
		std::string sceneName = scene->GetSceneNode()->GetEntity()->GetName();

		json["Scene"]["Name"] = sceneName;

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
		for (auto& child : scene->GetSceneNode()->GetChildren())
			json["Scene"]["RootNode"] = SerializeNode(json["Scene"]["RootNode"], child);

		std::string filePath = "..\\..\\..\\Assets\\SavedScenes\\" + std::string(sceneName) + ".scene";
		std::ofstream o(filePath);
		o << std::setw(4) << json << std::endl;

		//auto framebufferImage = scene->GetEditorCamera()->GetFramebuffer()->GetFrameBufferImage(0, 219);
		//WritePixelsToFile(framebufferImage, "..\\..\\..\\Resources\\SavedSceneThumbnails\\" + std::string(sceneName) + ".png");
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
		editorCamera->GetNode()->InitializeTransformMatrices();

		// Scene Cameras
		std::vector<Ref<SceneCamera>> sceneCameras = {};

		int numSceneCameras = jsonData["Scene"]["SceneCameras"].size();
		for (int i = 0; i < numSceneCameras; i++)
		{
			// Scene Camera Name
			std::string name = jsonData["Scene"]["SceneCameras"][i]["Name"];
			Ref<SceneCamera> sceneCamera = Factory::GetInstance()->InstantitateSceneCamera(name, editorCamera);
			
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

		Ref<Scene> scene = CreateRef<Scene>(sceneName, editorCamera, sceneCameras);
		SceneManager::GetInstance()->SetCurrentScene(scene);	

		// Iterate through all the children nodes for root node
		int numChildren = jsonData["Scene"]["RootNode"].size();
		for (int i = 0; i < numChildren; i++)
		{
			EntityType entityType = (EntityType)jsonData["Scene"]["RootNode"][i]["EntityType"];
		}
	}

	nlohmann::json::value_type SceneSerializer::SerializeNode(nlohmann::json::value_type jNode, Ref<Node> node)
	{
		const char* nodeName = node->GetEntity()->GetName().c_str();

		jNode[nodeName]["Name"] = nodeName;
		jNode[nodeName]["Enabled"] = node->m_Enabled;

		jNode[nodeName]["EntityType"] = node->GetEntity()->GetEntityType();

		if (node->GetEntity()->GetEntityType() == EntityType::PRIMITIVE)
		{
			jNode[nodeName]["PrimitiveType"] = node->GetMeshes()[0]->GetPrimitiveType();
		}
		else if (node->GetEntity()->GetEntityType() == EntityType::MODEL)
		{
			if (node->GetModelPath() != "")
				jNode[nodeName]["ModelPath"] = node->GetModelPath();
		}

		// Mesh and Material
		if (node->GetEntity()->GetEntityType() == EntityType::PRIMITIVE || node->GetEntity()->GetEntityType() == EntityType::MODEL)
		{
			for (int i = 0; i < node->GetMeshes().size(); i++)
			{
				std::string meshName = node->GetMeshes()[i]->GetName();
				Vector4 ambientColor = node->GetMeshes()[i]->GetMaterial()->GetAmbientColor();
				Vector4 diffuseColor = node->GetMeshes()[i]->GetMaterial()->GetDiffuseColor();
				Vector4 specularColor = node->GetMeshes()[i]->GetMaterial()->GetSpecularColor();

				jNode[nodeName]["Meshes"][i]["Name"] = meshName;

				jNode[nodeName]["Meshes"][i]["Material"]["Name"] = node->GetMeshes()[i]->GetMaterial()->GetName();
				jNode[nodeName]["Meshes"][i]["Material"]["Ambient"]["Color"] = { ambientColor.x, ambientColor.y, ambientColor.z, ambientColor.w };

				jNode[nodeName]["Meshes"][i]["Material"]["Diffuse"]["Color"] = { diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w };
				if (node->GetMeshes()[i]->GetMaterial()->GetDiffuseMap())
				{
					jNode[nodeName]["Meshes"][i]["Material"]["Diffuse"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetDiffuseMap()->GetPath();
				}
				jNode[nodeName]["Meshes"][i]["Material"]["Diffuse"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapOffset().y };
				jNode[nodeName]["Meshes"][i]["Material"]["Diffuse"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapTiling().y };

				jNode[nodeName]["Meshes"][i]["Material"]["Specular"]["Color"] = { specularColor.x, specularColor.y, specularColor.z, specularColor.w };
				if (node->GetMeshes()[i]->GetMaterial()->GetSpecularMap())
				{
					jNode[nodeName]["Meshes"][i]["Material"]["Specular"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetSpecularMap()->GetPath();
				}
				jNode[nodeName]["Meshes"][i]["Material"]["Specular"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetSpecularMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetSpecularMapOffset().y };
				jNode[nodeName]["Meshes"][i]["Material"]["Specular"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetSpecularMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetSpecularMapTiling().y };
				jNode[nodeName]["Meshes"][i]["Material"]["Shininess"] = node->GetMeshes()[i]->GetMaterial()->GetShininess();

				if (node->GetMeshes()[i]->GetMaterial()->GetNormalMap())
				{
					jNode[nodeName]["Meshes"][i]["Material"]["Normal"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetNormalMap()->GetPath();
				}
				jNode[nodeName]["Meshes"][i]["Material"]["Normal"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetNormalMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetNormalMapOffset().y };
				jNode[nodeName]["Meshes"][i]["Material"]["Normal"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetNormalMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetNormalMapTiling().y };
				jNode[nodeName]["Meshes"][i]["Material"]["Normal"]["BumpValue"] = node->GetMeshes()[i]->GetMaterial()->GetBumpValue();
			}
		}

		//Transform
		jNode[nodeName]["Transform"] =
		{
			{"LocalPosition", {node->GetTransform()->GetLocalPosition().x, node->GetTransform()->GetLocalPosition().y, node->GetTransform()->GetLocalPosition().z}},
			{"LocalEulerAngles", {node->GetTransform()->GetLocalEulerAngles().x, node->GetTransform()->GetLocalEulerAngles().y, node->GetTransform()->GetLocalEulerAngles().z}},
			{"LocalScale", {node->GetTransform()->GetLocalScale().x, node->GetTransform()->GetLocalScale().y, node->GetTransform()->GetLocalScale().z}}
		};

		for (int i = 0; i < node->GetChildren().size(); i++)
		{
			if (node->GetChildren()[i]->IsVisibleInEditor())
				jNode[nodeName] = SerializeNode(jNode[nodeName], node->GetChildren()[i]);
		}

		return jNode;
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
	}

	Ref<Node> SceneSerializer::DeserializeNode(nlohmann::json jsonNode)
	{
		std::string name;
		//int entityID;
		float localPosition[3];
		float localEulerAngles[3];
		float localScale[3];
		bool hasObject;
		std::string gameObjectName;
		//PrimitiveType primitiveType;
		bool isRendererActive;
		std::string materialName;
		float ambientColor[3];
		float diffuseColor[3];
		float specularColor[3];
		float shininess;
		float opacity;

		//Fetch node name
		jsonNode.at("Name").get_to(name);
		//jsonNode.at("EntityID").get_to(entityID);

		//Fetch node transform
		jsonNode.at("Transform").at("LocalPosition").get_to(localPosition);
		jsonNode.at("Transform").at("LocalEulerAngles").get_to(localEulerAngles);
		jsonNode.at("Transform").at("LocalScale").get_to(localScale);

		Ref<Node> node = nullptr;

		jsonNode.at("HasObject").get_to(hasObject);

		if (hasObject)
		{
			jsonNode.at("GameObject").at("Name").get_to(gameObjectName);
			//jsonNode.at("GameObject").at("PrimitiveType").get_to(primitiveType);
			jsonNode.at("GameObject").at("IsRendererActive").get_to(isRendererActive);
			jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Name").get_to(materialName);
			jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Ambient").get_to(ambientColor);
			jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Diffuse").get_to(diffuseColor);
			jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Specular").get_to(specularColor);
			jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Shininess").get_to(shininess);
			jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Opacity").get_to(opacity);

			/*node = TS_ENGINE::Factory::GetInstance()->CreatePrimitive(primitiveType, gameObjectName.c_str());

			node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetName(materialName.c_str());
			node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetAmbientColor(ambientColor[0], ambientColor[1], ambientColor[2]);
			node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetDiffuseColor(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
			node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetSpecularColor(specularColor[0], specularColor[1], specularColor[2]);
			node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetShininess(shininess);
			node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetOpacity(opacity);

			node->GetGameObject()->IsMeshRendererActive = isRendererActive;*/
		}
		else
		{
			//node = new TS_ENGINE::SceneManager::Node();
		}

		//Set node name
		//node->GetEntity()->SetName(name.c_str());

		//Set node tansform
		node->GetTransform()->SetLocalPosition(localPosition);
		node->GetTransform()->SetLocalEulerAngles(localEulerAngles);
		node->GetTransform()->SetLocalScale(localScale);

		/*if (jsonNode.at("Children") != NULL)
		{
			for (auto child : jsonNode.at("Children"))
				node->AddChild(DeserializeNode(child));
		}*/

		return node;
	}
}
