#include "tspch.h"
#include "Cone.h"

namespace TS_ENGINE {

	Cone::Cone()
	{

	}

	Cone::~Cone()
	{

	}

	void Cone::CreateMesh(Ref<Mesh> mesh, float radius, float height, int numSegments)
	{
        float segmentAngle = 2 * mPI / numSegments;

        // Generate vertex for the cone's apex
        {
            Vector3 pos;
            Vector3 normal;
            Vector2 texCoord;

            pos.x = 0.0f;
            pos.y = height / 2.0f;
            pos.z = 0.0f;
            normal.x = 0.0f;
            normal.y = 1.0f;
            normal.z = 0.0f;
            texCoord.x = 0.5f;
            texCoord.y = 0.5f;

            mesh->AddVertex(Vertex(pos, normal, texCoord));
        }

        // Generate base vertices
        for (int i = 0; i < numSegments; ++i) 
        {
            float angle = i * segmentAngle;
            float x = radius * cos(angle);
            float z = radius * sin(angle);

            Vector3 pos;
            Vector3 normal;
            Vector2 texCoord;

            pos.x = x;
            pos.y = -height / 2.0f;
            pos.z = z;
            normal.x = 0.0f;
            normal.y = -1.0f;
            normal.z = 0.0f;
            texCoord.x = (x / radius + 1.0f) * 0.5f;
            texCoord.y = (z / radius + 1.0f) * 0.5f;

            mesh->AddVertex(Vertex(pos, normal, texCoord));
        }

		// Generate indices for base
		for (int i = 1; i <= numSegments; ++i) 
		{
			mesh->AddIndex(0);
			mesh->AddIndex(i);
			mesh->AddIndex((i % numSegments) + 1);
		}
	}

	Ref<Mesh> Cone::GetMesh()
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetPrimitiveType(PrimitiveType::CONE);

		CreateMesh(mesh, mRadius, mHeight, mNumSegments);
		mesh->Create();
		return mesh;
	}
}