#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE {

	class Cube
	{
	public:
		Cube();		
		~Cube();

		Ref<StaticMesh> GetMesh();
		Ref<StaticMesh> GetMesh(const Matrix4& transformationMatrix);
	};
}

