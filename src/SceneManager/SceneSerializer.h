#pragma once
#include "Node.h"
//#include "ShaderManager/ShaderProgram.h"
#include <nlohmann/json.hpp>
//#include "Factory.h"

namespace TS_ENGINE
{
	class SceneSerializer
	{
	public:
		
		void Save(Ref<Scene> scene);
		nlohmann::json::value_type SerializeNode(nlohmann::json::value_type jNode, Ref<Node> node);

		std::string ReadSceneFromJson(std::string path);
		Ref<Node> DeserializeNode(nlohmann::json jsonNode);
		void Load(const nlohmann::json& j, Ref<Scene> scene);		
	};
}

