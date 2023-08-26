#include "tspch.h"
#include "Line.h"

namespace TS_ENGINE
{
	Line::Line()
	{
				
	}

	Line::~Line()
	{

	}

	Ref<Mesh> Line::GetMesh(const std::vector<Vector3>& points)
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();

		for (int i = 0; i < points.size(); i++)
		{
			mesh->AddVertex(Vertex(points[i]));
		}

		mesh->Create(DrawMode::LINE);

		return mesh;
	}
}
