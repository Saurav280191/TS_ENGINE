#include "tspch.h"
#include "SceneManager/Scene.h"
#include "SceneManager/SceneSerializer.h"
//#include <Factory.h>

namespace TS_ENGINE
{
	nlohmann::json::value_type SceneSerializer::SerializeNode(nlohmann::json::value_type jNode, Ref<Node> node)
	{
		const char* nodeName = node->GetName().c_str();

		jNode[nodeName] = {
			{"Name", nodeName}//,
			//{"EntityID", node->GetEntityID()},
			//{"HasObject", node->HasAttachedObject()}
		};

		if (node->HasAttachedObject())//GameObject
		{
			jNode[node->GetName()]["Object"];
			jNode[node->GetName()]["Object"]["Name"] = node->GetAttachedObject()->GetName();
			//jNode[node->GetName()]["Object"]["PrimitiveType"] = node->GetAttachedObject()->GetPrimitiveType();
			//jNode[node->GetName()]["Object"]["IsRendererActive"] = node->GetAttachedObject()->IsMeshRendererActive;

			//jNode[node->GetName()]["GameObject"]["MeshRenderer"]["MaterialProperties"] =
			{
				//{"Name", node->GetAttachedObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetName()},

				/*{
					"Ambient",
					{
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetAmbientColor().x,
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetAmbientColor().y,
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetAmbientColor().z
					}
				},*/

				/*{
					"Diffuse",
					{
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetDiffuseColor().x,
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetDiffuseColor().y,
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetDiffuseColor().z
					}
				},*/

				/*{
					"Specular",
					{
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetSpecularColor().x,
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetSpecularColor().y,
						node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetSpecularColor().z
					}
				},*/

				//{"Shininess", node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetShininess()},

				//{"Opacity", node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->GetOpacity()}
			};
		};

		//Transform
		jNode[node->GetName()]["Transform"] =
		{
			{"LocalPosition", {node->GetTransform()->GetLocalPosition().x, node->GetTransform()->GetLocalPosition().y, node->GetTransform()->GetLocalPosition().z}},
			{"LocalEulerAngles", {node->GetTransform()->GetLocalEulerAngles().x, node->GetTransform()->GetLocalEulerAngles().y, node->GetTransform()->GetLocalEulerAngles().z}},
			{"LocalScale", {node->GetTransform()->GetLocalScale().x, node->GetTransform()->GetLocalScale().y, node->GetTransform()->GetLocalScale().z}}
		};

		jNode[node->GetName()]["Children"];

		for (int i = 0; i < node->GetChildren().size(); i++)
		{
			jNode[node->GetName()]["Children"] = SerializeNode(jNode[node->GetName()]["Children"], node->GetChildren()[i]);
		}

		return jNode;
	}

	void SceneSerializer::Save(Ref<Scene> scene)
	{
		nlohmann::json json;
		std::string sceneName = scene->GetSceneNode()->GetName();

		json["Scene"] = {
		{"Name", sceneName}//,
		//{"EntityID", this->entityID},
		//{
			/*"Camera",
			{
				{"Name", scene->GetEditorCamera()->GetName()},
				{"EntityID", scene.GetEditorCamera()->GetEntityID()},
				{"ProjectionType", scene.GetEditorCamera()->GetProjectionType()},
				{
					"Perspective",
					{
							{"fov", scene.GetEditorCamera()->GetPerspective().fov},
							{"aspectRatio", scene.GetEditorCamera()->GetPerspective().aspectRatio},
							{"zNear", scene.GetEditorCamera()->GetPerspective().zNear},
							{"zFar", scene.GetEditorCamera()->GetPerspective().zFar}
					}
				},

				{
					"Transform",
					{
						{"LocalPosition", { scene.GetEditorCamera()->GetTransform()->GetLocalPosition().x, scene.GetEditorCamera()->GetTransform()->GetLocalPosition().y, scene.GetEditorCamera()->GetTransform()->GetLocalPosition().z}},
						{"LocalEulerAngles", { scene.GetEditorCamera()->GetTransform()->GetLocalEulerAngles().x,  scene.GetEditorCamera()->GetTransform()->GetLocalEulerAngles().y,  scene.GetEditorCamera()->GetTransform()->GetLocalEulerAngles().z}},
						{"LocalScale", { scene.GetEditorCamera()->GetTransform()->GetLocalScale().x,  scene.GetEditorCamera()->GetTransform()->GetLocalScale().y,  scene.GetEditorCamera()->GetTransform()->GetLocalScale().z}}
					}
				}
			}*/
		//}
		};

		//Serialize node recursively
		json = SerializeNode(json["Scene"], scene->GetSceneNode());

		std::string filePath = "SavedScenes\\" + std::string(sceneName) + ".json";
		std::ofstream o(filePath);
		o << std::setw(4) << json << std::endl;
	}

	//std::string SceneSerializer::ReadSceneFromJson(std::string path)
	//{
	//	std::string finalText;
	//	std::string line;
	//	std::ifstream jsonFileStream(path);

	//	if (jsonFileStream.is_open())
	//	{
	//		while (getline(jsonFileStream, line))
	//		{
	//			//std::cout << line << '\n';
	//			finalText += line;
	//		}
	//		jsonFileStream.close();
	//	}
	//	else if (jsonFileStream.fail())
	//	{
	//		std::cout << "Invalid path -> " << path << std::endl;
	//	}

	//	TS_CORE_INFO("ReadSceneFromJson!");
	//	return finalText;
	//}

	//Node* SceneSerializer::DeserializeNode(nlohmann::json jsonNode)
	//{
	//	std::string name;
	//	int entityID;
	//	float localPosition[3];
	//	float localEulerAngles[3];
	//	float localScale[3];
	//	bool hasGameObject;
	//	std::string gameObjectName;
	//	PrimitiveType primitiveType;
	//	bool isRendererActive;
	//	std::string materialName;
	//	float ambientColor[3];
	//	float diffuseColor[3];
	//	float specularColor[3];
	//	float shininess;
	//	float opacity;

	//	//Fetch node name
	//	jsonNode.at("Name").get_to(name);
	//	jsonNode.at("EntityID").get_to(entityID);

	//	//Fetch node transform
	//	jsonNode.at("Transform").at("LocalPosition").get_to(localPosition);
	//	jsonNode.at("Transform").at("LocalEulerAngles").get_to(localEulerAngles);
	//	jsonNode.at("Transform").at("LocalScale").get_to(localScale);

	//	Ref<Node> node = nullptr;

	//	jsonNode.at("HasGameObject").get_to(hasGameObject);

	//	if (hasGameObject)
	//	{
	//		jsonNode.at("GameObject").at("Name").get_to(gameObjectName);
	//		jsonNode.at("GameObject").at("PrimitiveType").get_to(primitiveType);
	//		jsonNode.at("GameObject").at("IsRendererActive").get_to(isRendererActive);
	//		jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Name").get_to(materialName);
	//		jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Ambient").get_to(ambientColor);
	//		jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Diffuse").get_to(diffuseColor);
	//		jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Specular").get_to(specularColor);
	//		jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Shininess").get_to(shininess);
	//		jsonNode.at("GameObject").at("MeshRenderer").at("MaterialProperties").at("Opacity").get_to(opacity);

	//		node = TS_ENGINE::Factory::GetInstance()->CreatePrimitive(primitiveType, gameObjectName.c_str());

	//		node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetName(materialName.c_str());
	//		node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetAmbientColor(ambientColor[0], ambientColor[1], ambientColor[2]);
	//		node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetDiffuseColor(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
	//		node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetSpecularColor(specularColor[0], specularColor[1], specularColor[2]);
	//		node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetShininess(shininess);
	//		node->GetGameObject()->GetMeshRenderer()->GetMaterial()->GetMaterialProperties()->SetOpacity(opacity);

	//		node->GetGameObject()->IsMeshRendererActive = isRendererActive;
	//	}
	//	else
	//	{
	//		node = new TS_ENGINE::SceneManager::Node();
	//	}

	//	//Set node name
	//	node->SetName(name.c_str());

	//	//Set node tansform
	//	node->GetTransform()->SetLocalPosition(localPosition);
	//	node->GetTransform()->SetLocalEulerAngles(localEulerAngles);
	//	node->GetTransform()->SetLocalScale(localScale);

	//	if (jsonNode.at("Children") != NULL)
	//	{
	//		for (auto child : jsonNode.at("Children"))
	//			node->AddChild(DeserializeNode(child));
	//	}

	//	return node;
	//}

	//void SceneSerializer::Load(const nlohmann::json& j, TS_ENGINE::SceneManager::Scene& scene)
	//{
	//	std::string sceneName;
	//	std::string editorCameraName;
	//	TS_ENGINE::CameraNew* editorCamera = new TS_ENGINE::CameraNew(TS_ENGINE::CameraNew::EDITORCAMERA);
	//	TS_ENGINE::CameraNew::ProjectionType projectionType;
	//	TS_ENGINE::CameraNew::Perspective perspective;
	//	TS_ENGINE::CameraNew::Orthographic orthographic;

	//	j.at("Name").get_to(sceneName);
	//	//j.at("EntityID").get_to(scene.enityID);
	//	j.at("Camera").at("Name").get_to(editorCameraName);
	//	//j.at("Camera").at("EntityID").get_to(camera->enityID);
	//	j.at("Camera").at("ProjectionType").get_to(projectionType);

	//	if (projectionType == TS_ENGINE::CameraNew::PERSPECTIVE)
	//	{
	//		float fov, aspectRatio, zNear, zFar;
	//		j.at("Camera").at("Perspective").at("fov").get_to(fov);
	//		j.at("Camera").at("Perspective").at("aspectRatio").get_to(aspectRatio);
	//		j.at("Camera").at("Perspective").at("zNear").get_to(zNear);
	//		j.at("Camera").at("Perspective").at("zFar").get_to(zFar);
	//		perspective = TS_ENGINE::CameraNew::Perspective(fov, aspectRatio, zNear, zFar);
	//	}
	//	else if (projectionType == TS_ENGINE::CameraNew::ProjectionType::ORTHOGRAPHIC)
	//	{
	//		float left, right, top, bottom, zNear, zFar = 0;
	//		j.at("Camera").at("Orthographic").at("fov").get_to(left);
	//		j.at("Camera").at("Orthographic").at("aspectRatio").get_to(right);
	//		j.at("Camera").at("Orthographic").at("fov").get_to(top);
	//		j.at("Camera").at("Orthographic").at("fov").get_to(bottom);
	//		j.at("Camera").at("Orthographic").at("zNear").get_to(zNear);
	//		j.at("Camera").at("Orthographic").at("zFar").get_to(zFar);
	//		orthographic = TS_ENGINE::CameraNew::Orthographic(left, right, top, bottom, zNear, zFar);
	//	}

	//	float localPosition[3];
	//	j.at("Camera").at("Transform").at("LocalPosition").get_to(localPosition);
	//	float localEulerAngles[3];
	//	j.at("Camera").at("Transform").at("LocalEulerAngles").get_to(localEulerAngles);
	//	float localScale[3];
	//	j.at("Camera").at("Transform").at("LocalScale").get_to(localScale);

	//	scene.SetName(sceneName.c_str());
	//	editorCamera->SetName(editorCameraName.c_str());

	//	if (projectionType == TS_ENGINE::CameraNew::PERSPECTIVE)
	//		editorCamera->SetPerspective(perspective);
	//	else if (projectionType == TS_ENGINE::CameraNew::ORTHOGRAPHIC)
	//		editorCamera->SetOrthographic(orthographic);

	//	scene.SetEditorCamera(editorCamera);

	//	nlohmann::json rootJsonNode = j.at("RootNode");

	//	if (rootJsonNode.at("Children") != NULL)
	//	{
	//		for (auto child : rootJsonNode.at("Children"))
	//			scene.GetNode()->AddChild(DeserializeNode(child));
	//	}

	//	TS_CORE_INFO("Deserialized Scene!");
	//}
}
