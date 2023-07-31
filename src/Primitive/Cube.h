#pragma once
#include "GameObject.h"

namespace TS_ENGINE {

	class Cube : public GameObject
	{
	private:
		Ref<Mesh> mMesh;
	public:
		Cube(const std::string& name);
		Cube(const std::string& name, const Vector3& color);
		void Create();
		void Create(const Matrix4& transformationMatrix);
		void Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	};
}

