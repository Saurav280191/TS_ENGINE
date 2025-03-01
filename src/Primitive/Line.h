#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE
{
	class Line
	{
	public:
		Line();
		~Line();

		void CreateMesh(const std::vector<Vector3>& points);
		Ref<StaticMesh> GetMesh();
	private:
		Ref<StaticMesh> mMesh;
	};
}

