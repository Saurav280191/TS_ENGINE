#include "tspch.h"
#include "Cylinder.h"

namespace TS_ENGINE {

	Cylinder::Cylinder()
	{

	}

	Cylinder::~Cylinder()
	{

	}

	void Cylinder::CreateMesh(Ref<Mesh> mesh, float radius, float height, int numSegments)
	{
		float segmentAngle = 2 * mPI / numSegments;

		// Generate vertices for side
		for (int i = 0; i <= numSegments; ++i)
		{
			float angle = i * segmentAngle;

			float x = radius * cos(angle);
			float z = radius * sin(angle);
			float s = static_cast<float>(i) / numSegments;

			for (int j = 0; j <= 1; ++j)
			{
				float y = (j == 0) ? -height / 2.0f : height / 2.0f;
				float t = static_cast<float>(j);
				
				Vector3 pos;
				Vector3 normal;
				Vector2 texCoord;

				pos.x = x;
				pos.y = y;
				pos.z = z;
				normal.x = x;
				normal.y = 0.0f;
				normal.z = z;
				texCoord.x = s;
				texCoord.y = t;

				mesh->AddVertex(Vertex(pos, normal, texCoord));
			}
		}

		// Generate indices for side
		for (int i = 0; i < numSegments; ++i)
		{
			mesh->AddIndex(i * 2);
			mesh->AddIndex(i * 2 + 1);
			mesh->AddIndex((i + 1) * 2 + 1);

			mesh->AddIndex(i * 2);
			mesh->AddIndex((i + 1) * 2 + 1);
			mesh->AddIndex((i + 1) * 2);
		}
	}


	Ref<Mesh> Cylinder::GetMesh()
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetPrimitiveType(PrimitiveType::CYLINDER);

		CreateMesh(mesh, mRadius, mHeight, mNumSegments);

		mesh->Create();
		return mesh;
	}
}