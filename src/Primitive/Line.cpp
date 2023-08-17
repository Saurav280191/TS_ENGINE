#include "tspch.h"
#include "Line.h"

namespace TS_ENGINE
{
	Line::Line(const std::string& name)
	{
		//mPrimitiveType = PrimitiveType::LINE;
		mName = name;
		mMesh = CreateRef<Mesh>();
		Initialize();		
	}

	void Line::Create(const std::vector<Vector3>& points)
	{
		for(int i = 0; i < points.size(); i++)
			mMesh->AddVertex(Vertex(points[i], Vector3(0, 0, -1), Vector2(0, 0)));
		
		mMesh->Create(DrawMode::LINE);
		AddMesh(mMesh);
	}

	/*void Line::Draw(const std::vector<Vector4>& points)
	{
		mMesh->GetVertices().clear();

		for (int i = 0; i < points.size(); i++)
			mMesh->AddVertex(Vertex((Vector3)points[i], GetColor(), Vector3(0, 0, -1), Vector2(0, 0)));

		mMesh->Create(DrawMode::LINE);
		AddMesh(mMesh);
	}*/
}
