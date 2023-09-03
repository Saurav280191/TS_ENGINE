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

		// Editor Camera
		{
			//Transform
			json[sceneName]["EditorCamera"]["Transform"] = {
				{ "LocalPosition", {scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalPosition().x, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalPosition().y, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalPosition().z}},
				{ "LocalEulerAngles", {scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalEulerAngles().x, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalEulerAngles().y, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalEulerAngles().z}},
				{ "LocalScale", {scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalScale().x, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalScale().y, scene->GetEditorCamera()->GetNode()->GetTransform()->GetLocalScale().z}}
			};

			//Projection
			if (scene->GetEditorCamera()->GetProjectionType() == Camera::ProjectionType::PERSPECTIVE)// Perspective 
			{
				json[sceneName]["EditorCamera"]["Projection"] = {
					{"ProjectionType", "Perspective"},
					{"fov", scene->GetEditorCamera()->GetPerspective().fov},
					{"aspectRatio", scene->GetEditorCamera()->GetPerspective().aspectRatio},
					{"zNear", scene->GetEditorCamera()->GetPerspective().zNear},
					{"zFar", scene->GetEditorCamera()->GetPerspective().zFar},
				};
			}
			else if (scene->GetEditorCamera()->GetProjectionType() == Camera::ProjectionType::ORTHOGRAPHIC) // Orthographic
			{
				json[sceneName]["EditorCamera"]["Projection"] = {
					{"ProjectionType", "Orthographic"},
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
				json[sceneName]["SceneCameras"][i]["Name"] = scene->GetSceneCameras()[i]->GetNode()->GetEntity()->GetName();

				// Transform
				json[sceneName]["SceneCameras"][i]["Transform"] =
				{
					{ "LocalPosition", { scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalPosition().x, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalPosition().y, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalPosition().z} },
					{ "LocalEulerAngles", { scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalEulerAngles().x, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalEulerAngles().y, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalEulerAngles().z} },
					{ "LocalScale", { scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalScale().x, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalScale().y, scene->GetSceneCameras()[i]->GetNode()->GetTransform()->GetLocalScale().z} },
				};

				// Projection
				if (scene->GetSceneCameras()[i]->GetProjectionType() == Camera::ProjectionType::PERSPECTIVE)// Perspective 
				{
					json[sceneName]["SceneCameras"][i]["Projection"] = {
						{"ProjectionType", "Perspective"},
						{"fov", scene->GetSceneCameras()[i]->GetPerspective().fov},
						{"aspectRatio", scene->GetSceneCameras()[i]->GetPerspective().aspectRatio},
						{"zNear", scene->GetSceneCameras()[i]->GetPerspective().zNear},
						{"zFar", scene->GetSceneCameras()[i]->GetPerspective().zFar},
					};
				}
				else if (scene->GetSceneCameras()[i]->GetProjectionType() == Camera::ProjectionType::ORTHOGRAPHIC) // Orthographic
				{
					json[sceneName]["SceneCameras"][i]["Projection"] = {
						{"ProjectionType", "Orthographic"},
						{"size", scene->GetSceneCameras()[i]->GetOrthographic().top},
						{"zNear", scene->GetSceneCameras()[i]->GetOrthographic().zNear},
						{"zFar", scene->GetSceneCameras()[i]->GetOrthographic().zFar},
					};
				}
			}
		}

		// Nodes (Recursively iterate through scene node children and serialize them)	
		for (auto& child : scene->GetSceneNode()->GetChildren())
			json[sceneName]["RootNode"] = SerializeNode(json[sceneName]["RootNode"], child);

		std::string filePath = "..\\..\\..\\Assets\\SavedScenes\\" + std::string(sceneName) + ".scene";
		std::ofstream o(filePath);
		o << std::setw(4) << json << std::endl;

		//auto framebufferImage = scene->GetEditorCamera()->GetFramebuffer()->GetFrameBufferImage(0, 219);
		//WritePixelsToFile(framebufferImage, "..\\..\\..\\Resources\\SavedSceneThumbnails\\" + std::string(sceneName) + ".png");
	}

	void SceneSerializer::Load(const std::string& savedScenePath)
	{
		TS_CORE_TRACE("Loading: {0}", savedScenePath);

		//std::string sceneName;
		//std::string editorCameraName;
		//TS_ENGINE::CameraNew* editorCamera = new TS_ENGINE::CameraNew(TS_ENGINE::CameraNew::EDITORCAMERA);
		//TS_ENGINE::CameraNew::ProjectionType projectionType;
		//TS_ENGINE::CameraNew::Perspective perspective;
		//TS_ENGINE::CameraNew::Orthographic orthographic;

		//j.at("Name").get_to(sceneName);
		////j.at("EntityID").get_to(scene.enityID);
		//j.at("Camera").at("Name").get_to(editorCameraName);
		////j.at("Camera").at("EntityID").get_to(camera->enityID);
		//j.at("Camera").at("ProjectionType").get_to(projectionType);

		//if (projectionType == TS_ENGINE::CameraNew::PERSPECTIVE)
		//{
		//	float fov, aspectRatio, zNear, zFar;
		//	j.at("Camera").at("Perspective").at("fov").get_to(fov);
		//	j.at("Camera").at("Perspective").at("aspectRatio").get_to(aspectRatio);
		//	j.at("Camera").at("Perspective").at("zNear").get_to(zNear);
		//	j.at("Camera").at("Perspective").at("zFar").get_to(zFar);
		//	perspective = TS_ENGINE::CameraNew::Perspective(fov, aspectRatio, zNear, zFar);
		//}
		//else if (projectionType == TS_ENGINE::CameraNew::ProjectionType::ORTHOGRAPHIC)
		//{
		//	float left, right, top, bottom, zNear, zFar = 0;
		//	j.at("Camera").at("Orthographic").at("fov").get_to(left);
		//	j.at("Camera").at("Orthographic").at("aspectRatio").get_to(right);
		//	j.at("Camera").at("Orthographic").at("fov").get_to(top);
		//	j.at("Camera").at("Orthographic").at("fov").get_to(bottom);
		//	j.at("Camera").at("Orthographic").at("zNear").get_to(zNear);
		//	j.at("Camera").at("Orthographic").at("zFar").get_to(zFar);
		//	orthographic = TS_ENGINE::CameraNew::Orthographic(left, right, top, bottom, zNear, zFar);
		//}

		//float localPosition[3];
		//j.at("Camera").at("Transform").at("LocalPosition").get_to(localPosition);
		//float localEulerAngles[3];
		//j.at("Camera").at("Transform").at("LocalEulerAngles").get_to(localEulerAngles);
		//float localScale[3];
		//j.at("Camera").at("Transform").at("LocalScale").get_to(localScale);

		//scene.SetName(sceneName.c_str());
		//editorCamera->SetName(editorCameraName.c_str());

		//if (projectionType == TS_ENGINE::CameraNew::PERSPECTIVE)
		//	editorCamera->SetPerspective(perspective);
		//else if (projectionType == TS_ENGINE::CameraNew::ORTHOGRAPHIC)
		//	editorCamera->SetOrthographic(orthographic);

		//scene.SetEditorCamera(editorCamera);

		//nlohmann::json rootJsonNode = j.at("RootNode");

		//if (rootJsonNode.at("Children") != NULL)
		//{
		//	for (auto child : rootJsonNode.at("Children"))
		//		scene.GetNode()->AddChild(DeserializeNode(child));
		//}

		//TS_CORE_INFO("Deserialized Scene!");
	}
	
	nlohmann::json::value_type SceneSerializer::SerializeNode(nlohmann::json::value_type jNode, Ref<Node> node)
	{
		const char* nodeName = node->GetEntity()->GetName().c_str();

		jNode[nodeName]["Name"] = nodeName;
		jNode[nodeName]["Enabled"] = node->m_Enabled;

		switch (node->GetEntity()->GetEntityType())
		{
		case PRIMITIVE:
		{
			jNode[nodeName]["EntityType"] = "Primitive";

			if (node->GetMeshes()[0]->GetPrimitiveType() == PrimitiveType::LINE)
			{
				jNode[nodeName]["PrimitiveType"] = "Line";
			}
			else if (node->GetMeshes()[0]->GetPrimitiveType() == PrimitiveType::QUAD)
			{
				jNode[nodeName]["PrimitiveType"] = "Quad";
			}
			else if (node->GetMeshes()[0]->GetPrimitiveType() == PrimitiveType::CUBE)
			{
				jNode[nodeName]["PrimitiveType"] = "Cube";
			}
			else if (node->GetMeshes()[0]->GetPrimitiveType() == PrimitiveType::SPHERE)
			{
				jNode[nodeName]["PrimitiveType"] = "Sphere";
			}
			else if (node->GetMeshes()[0]->GetPrimitiveType() == PrimitiveType::CYLINDER)
			{
				jNode[nodeName]["PrimitiveType"] = "Cylinder";
			}
			else if (node->GetMeshes()[0]->GetPrimitiveType() == PrimitiveType::CONE)
			{
				jNode[nodeName]["PrimitiveType"] = "Cone";
			}
			else if (node->GetMeshes()[0]->GetPrimitiveType() == PrimitiveType::MODEL)
			{
				jNode[nodeName]["PrimitiveType"] = "Model";
			}
		}
		break;

		case MODEL:
		{
			jNode[nodeName]["EnityType"] = "Model";
			
			if (node->GetModelPath() != "")
				jNode[nodeName]["ModelPath"] = node->GetModelPath();
		}
		break;

		case CAMERA:
		{
			jNode[nodeName]["EnityType"] = "Camera";
		}
		break;

		case SKYBOX:
		{
			jNode[nodeName]["EnityType"] = "Skybox";
		}
		break;

		case SCENE:
		{
			jNode[nodeName]["EnityType"] = "Scene";
		}
		break;

		case LIGHT:
		{
			jNode[nodeName]["EnityType"] = "Light";
		}
		break;

		case EMPTY:
		{
			jNode[nodeName]["EntityType"] = "Default";
		}
		break;
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

	std::string SceneSerializer::ReadSceneFromJson(std::string path)
	{
		std::string finalText;
		std::string line;
		std::ifstream jsonFileStream(path);

		if (jsonFileStream.is_open())
		{
			while (getline(jsonFileStream, line))
			{
				//std::cout << line << '\n';
				finalText += line;
			}
			jsonFileStream.close();
		}
		else if (jsonFileStream.fail())
		{
			TS_CORE_ERROR("Invalid path -> {0}", path);
		}

		TS_CORE_INFO("ReadSceneFromJson!");
		return finalText;
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
