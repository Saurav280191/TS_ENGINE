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
		SerializeNode(scene->GetSceneNode(), json["Scene"]["RootNode"]);

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

		for (int i = 0; i < jsonData["Scene"]["SceneCameras"].size(); i++)
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
		{
			int numRootChildren = jsonData["Scene"]["RootNode"]["Children"].size();
			for (int i = 0; i < numRootChildren; i++)
			{
				nlohmann::json jsonNode = jsonData["Scene"]["RootNode"]["Children"][i];
				DeserializeNode(scene->GetSceneNode(), jsonNode, editorCamera);
			}
		}
	}

	void SceneSerializer::SerializeNode(Ref<Node> node, nlohmann::json& jsonNode)
	{
		jsonNode["Name"] = node->GetEntity()->GetName().c_str();
		jsonNode["Enabled"] = node->m_Enabled;
		jsonNode["EntityType"] = node->GetEntity()->GetEntityType();

		// EntityType And PrimitiveType
		if (node->GetEntity()->GetEntityType() == EntityType::PRIMITIVE)
		{
			jsonNode["PrimitiveType"] = node->GetMeshes()[0]->GetPrimitiveType();
		}
		else if (node->GetEntity()->GetEntityType() == EntityType::MODEL)
		{
			if (node->GetModelPath() != "")
				jsonNode["ModelPath"] = node->GetModelPath().c_str();
		}

		// Mesh And Material
		if (node->GetEntity()->GetEntityType() == EntityType::PRIMITIVE || node->GetEntity()->GetEntityType() == EntityType::MODEL)
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
				if (node->GetMeshes()[i]->GetMaterial()->GetDiffuseMap())
				{
					jsonNode["Meshes"][i]["Material"]["Diffuse"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetDiffuseMap()->GetPath();
				}
				jsonNode["Meshes"][i]["Material"]["Diffuse"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapOffset().y };
				jsonNode["Meshes"][i]["Material"]["Diffuse"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetDiffuseMapTiling().y };

				jsonNode["Meshes"][i]["Material"]["Specular"]["Color"] = { specularColor.x, specularColor.y, specularColor.z, specularColor.w };
				if (node->GetMeshes()[i]->GetMaterial()->GetSpecularMap())
				{
					jsonNode["Meshes"][i]["Material"]["Specular"]["TexturePath"] = node->GetMeshes()[i]->GetMaterial()->GetSpecularMap()->GetPath();
				}
				jsonNode["Meshes"][i]["Material"]["Specular"]["TextureOffset"] = { node->GetMeshes()[i]->GetMaterial()->GetSpecularMapOffset().x, node->GetMeshes()[i]->GetMaterial()->GetSpecularMapOffset().y };
				jsonNode["Meshes"][i]["Material"]["Specular"]["TextureTiling"] = { node->GetMeshes()[i]->GetMaterial()->GetSpecularMapTiling().x, node->GetMeshes()[i]->GetMaterial()->GetSpecularMapTiling().y };
				jsonNode["Meshes"][i]["Material"]["Specular"]["Shininess"] = node->GetMeshes()[i]->GetMaterial()->GetShininess();

				if (node->GetMeshes()[i]->GetMaterial()->GetNormalMap())
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
		for (Ref<Node> childNode : node->GetChildren())
		{
			nlohmann::json childJson;
			SerializeNode(childNode, childJson);
			jsonNode["Children"].push_back(childJson);
		}
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

	void SceneSerializer::DeserializeNode(Ref<Node> parentNode, nlohmann::json& jsonNode, Ref<EditorCamera> editorCamera)
	{
		std::string name = jsonNode["Name"];
		EntityType entityType = (EntityType)jsonNode["EntityType"];
		bool enabled = jsonNode["Enabled"];

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

		Ref<Node> node = nullptr;

		if (entityType == EntityType::PRIMITIVE)
		{
			PrimitiveType primitiveType = (PrimitiveType)jsonNode["PrimitiveType"];

			switch (primitiveType)
			{
			case PrimitiveType::QUAD:
				node = Factory::GetInstance()->InstantiateQuad(name, parentNode);
				break;
			case PrimitiveType::CUBE:
				node = Factory::GetInstance()->InstantiateCube(name, parentNode);
				break;
			case PrimitiveType::SPHERE:
				node = Factory::GetInstance()->InstantiateSphere(name, parentNode);
				break;
			case PrimitiveType::CYLINDER:
				node = Factory::GetInstance()->InstantiateCylinder(name, parentNode);
				break;
			case PrimitiveType::CONE:
				node = Factory::GetInstance()->InstantiateCone(name, parentNode);
				break;
			}
		}
		else if (entityType == EntityType::MODEL)
		{
			auto it = jsonNode.find("ModelPath");

			if (it != jsonNode.end())
			{
				node = Factory::GetInstance()->InstantiateModel(jsonNode["ModelPath"], parentNode);
			}
		}
		else if (entityType == EntityType::CAMERA)
		{
			Ref<SceneCamera> sceneCamera = Factory::GetInstance()->InstantitateSceneCamera(name, editorCamera);
			node = sceneCamera->GetNode();// No need to set parent for camera it is already done in the scene creation process
		}
		else
		{
			Ref<Node> node = CreateRef<Node>();
			node->SetNodeRef(node);
			parentNode->AddChild(node);
		}

		if (node)
		{
			node->GetTransform()->SetLocalPosition(transformPosition);
			node->GetTransform()->SetLocalEulerAngles(transformEulerAngles);
			node->GetTransform()->SetLocalScale(transformScale);
			node->Initialize(name, entityType);
			node->m_Enabled = enabled;

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

				node->GetMeshes()[i]->GetMaterial()->SetDiffuseMap(diffuseTexture);
				node->GetMeshes()[i]->GetMaterial()->SetSpecularMap(diffuseTexture);
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
		}

		// Deserialize children
		if (jsonNode["Children"] != nullptr)//Making sure Children exist 
		{
			for (int i = 0; i < jsonNode["Children"].size(); i++)
			{
				nlohmann::json childJsonNode = jsonNode["Children"][i];
				DeserializeNode(node, childJsonNode, editorCamera);
			}
		}

	}
}
