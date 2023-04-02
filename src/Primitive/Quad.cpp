#include "tspch.h"
#include "Quad.h"

namespace TS_ENGINE {

	Quad::Quad(const char* name)
	{
		mName = name;
		mMesh = new Mesh();
	}

	Quad::Quad(const char* name, Vector3 color)
	{
		mName = name;
		mMesh = new Mesh();
		mColor = color;
	}

	void Quad::OverrideTexCoords(std::vector<Vector2> texCoords)
	{
		for (auto mesh : mMeshes)
			mesh->Destroy();
		mMeshes.clear();

		mMesh = new Mesh();

		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(texCoords[0].x, texCoords[0].y)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(texCoords[1].x, texCoords[1].y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(texCoords[2].x, texCoords[2].y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(texCoords[3].x, texCoords[3].y)));

		mMesh->AddIndex(0);
		mMesh->AddIndex(1);
		mMesh->AddIndex(3);
		mMesh->AddIndex(1);
		mMesh->AddIndex(2);
		mMesh->AddIndex(3);

		mMesh->Create();
		AddMesh(mMesh);
	}

	void Quad::Create()
	{
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(0, 0)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(0, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, 0)));

		mMesh->AddIndex(0);
		mMesh->AddIndex(1);
		mMesh->AddIndex(3);
		mMesh->AddIndex(1);
		mMesh->AddIndex(2);
		mMesh->AddIndex(3);

		mMesh->Create();
		AddMesh(mMesh);
	}

	void Quad::Create(const Matrix4& transformationMatrix)
	{
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(0, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(0, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.0f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, 0), transformationMatrix));

		mMesh->AddIndex(0);
		mMesh->AddIndex(1);
		mMesh->AddIndex(3);
		mMesh->AddIndex(1);
		mMesh->AddIndex(2);
		mMesh->AddIndex(3);

		mMesh->Create();
		AddMesh(mMesh);
	}
}