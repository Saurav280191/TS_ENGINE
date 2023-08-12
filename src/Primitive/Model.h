#pragma once
#include "Core/tspch.h"
#include "Mesh.h"
#include <stb_image.h>

namespace TS_ENGINE {

	class Model : public GameObject
	{
	private:
		uint32_t mRendererID;
		AssimpMaterial material;

		void ProcessNode(aiNode* node, const aiScene* scene);
		Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	public:
		Model();
		Model(const std::string& modelPath);		
		void LoadModel(const std::string& modelPath);

		void CopyFrom(Ref<Model> model);
		~Model();		
	};
}

