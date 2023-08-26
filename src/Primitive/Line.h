#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE
{
	class Line
	{
	private:

	public:
		Line();
		~Line();

		Ref<Mesh> GetMesh(const std::vector<Vector3>& points);
	};
}

