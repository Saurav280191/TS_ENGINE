#pragma once
#include "Node.h"
//#include "ShaderManager/ShaderProgram.h"
#include <nlohmann/json.hpp>
#include "Core/Factory.h"

namespace TS_ENGINE
{
	class SceneSerializer
	{
	public:
		SceneSerializer();
		~SceneSerializer();

		void Save(Ref<Scene> scene);
		void Load(const std::string& savedScenePath);
	private:
		void SerializeNode(Ref<Node> node, nlohmann::json& jsonNode);
		nlohmann::json GetJsonDataFromFile(const std::string& filePath);		
		void DeserializeNode(Ref<Node> parentNode, nlohmann::json& jsonNode, Ref<EditorCamera> editorCamera);
		void DeserializeModelNode(Ref<Node> node, Ref<Node> parentNode, nlohmann::json& jsonNode, Ref<EditorCamera> editorCamera);
		void ApplyAndDeserializeChildrenNode(Ref<Node> node, nlohmann::json& jsonNode, Ref<EditorCamera> editorCamera);
		void ApplyTransformAndMaterialForNode(Ref<Node> node, nlohmann::json& jsonNode);	
	};
}

