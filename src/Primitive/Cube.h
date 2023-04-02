#pragma once
#include "GameObject.h"

namespace TS_ENGINE {

	class Cube : public GameObject
	{
	private:
		Mesh* mMesh;
	public:
		Cube(const char* name);
		void Create();
		void Create(const Matrix4& transformationMatrix);
		void Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
	};
}

