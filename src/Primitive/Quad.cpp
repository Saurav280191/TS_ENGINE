#include "tspch.h"
#include "Quad.h"

namespace TS_ENGINE {

	Quad::Quad()
	{
		
	}

	Quad::~Quad()
	{
	}

	/*void Quad::OverrideTexCoords(std::vector<Vector2> texCoords)
	{
		for (auto mesh : mMeshes)
			mesh->~Mesh();

		mMeshes.clear();

		mMesh = CreateRef<Mesh>();

		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.0f), Vector3(0, 1, 0), Vector2(texCoords[0].x, texCoords[0].y)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.0f), Vector3(0, 1, 0), Vector2(texCoords[1].x, texCoords[1].y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.0f), Vector3(0, 1, 0), Vector2(texCoords[2].x, texCoords[2].y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.0f), Vector3(0, 1, 0), Vector2(texCoords[3].x, texCoords[3].y)));

		mMesh->AddIndex(0);
		mMesh->AddIndex(1);
		mMesh->AddIndex(3);
		mMesh->AddIndex(1);
		mMesh->AddIndex(2);
		mMesh->AddIndex(3);

		mMesh->Create();		
	}*/

	Ref<Mesh> Quad::GetMesh()
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetPrimitiveType(PrimitiveType::QUAD);
		
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.0f), Vector2(0, 0), Vector3(0, 1, 0)));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.0f), Vector2(0, 1), Vector3(0, 1, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.0f), Vector2(1, 1), Vector3(0, 1, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.0f), Vector2(1, 0), Vector3(0, 1, 0)));

		mesh->AddIndex(0);
		mesh->AddIndex(1);
		mesh->AddIndex(3);
		mesh->AddIndex(1);
		mesh->AddIndex(2);
		mesh->AddIndex(3);

		mesh->Create();
		
		return mesh;
	}

	Ref<Mesh> Quad::GetMesh(const Matrix4& transformationMatrix)
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();

		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.0f), Vector2(0, 0), Vector3(0, 1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.0f), Vector2(0, 1), Vector3(0, 1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.0f), Vector2(1, 1), Vector3(0, 1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.0f), Vector2(1, 0), Vector3(0, 1, 0), transformationMatrix));

		mesh->AddIndex(0);
		mesh->AddIndex(1);
		mesh->AddIndex(3);
		mesh->AddIndex(1);
		mesh->AddIndex(2);
		mesh->AddIndex(3);

		mesh->Create();
		
		return mesh;
	}
}