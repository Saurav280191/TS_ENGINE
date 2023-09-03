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
		void Save(Ref<Scene> scene);
		void Load(const std::string& savedScenePath);
	private:
		nlohmann::json::value_type SerializeNode(nlohmann::json::value_type jNode, Ref<Node> node);
		nlohmann::json GetJsonDataFromFile(const std::string& filePath);
		Ref<Node> DeserializeNode(nlohmann::json jsonNode);
	};
}

