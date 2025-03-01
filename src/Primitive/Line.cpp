#include "tspch.h"
#include "Line.h"

namespace TS_ENGINE
{
	Line::Line()
	{

	}

	Line::~Line()
	{
		TS_CORE_INFO("Destroyed Line!");
	}

	void Line::CreateMesh(const std::vector<Vector3>& points)
	{
		mMesh = CreateRef<StaticMesh>();
		mMesh->SetPrimitiveType(PrimitiveType::LINE);

		for (int i = 0; i < points.size(); i++)
		{
			mMesh->AddVertex(Vertex(points[i]));
		}

		mMesh->Create(DrawMode::LINE);
	}

	Ref<StaticMesh> Line::GetMesh()
	{
		return mMesh;
	}
}
