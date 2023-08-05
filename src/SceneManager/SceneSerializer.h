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
		
		nlohmann::json::value_type SerializeNode(nlohmann::json::value_type jNode, Ref<Node> node);

		//Node* DeserializeNode(nlohmann::json jsonNode);

		void Save(Ref<Scene> scene);

		//std::string ReadSceneFromJson(std::string path);

		//void Load(const nlohmann::json& j, Ref<Scene> scene);		
	};
}

