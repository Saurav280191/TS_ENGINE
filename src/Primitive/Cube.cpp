#include "tspch.h"
#include "Cube.h"

namespace TS_ENGINE {

	Cube::Cube(const std::string& name)
	{
		mPrimitiveType = PrimitiveType::CUBE;
		mName = name;
		mMesh = CreateRef<Mesh>();
		Initialize();
	}

	Cube::Cube(const std::string& name, const Vector3& color)
	{
		mPrimitiveType = PrimitiveType::CUBE;
		mName = name;
		mColor = color;
		mMesh = CreateRef<Mesh>();
		Initialize();
	}

	void Cube::Create()
	{
		//Front
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(mTiling.x, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(mTiling.x, 0)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(0, 0)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(0, mTiling.y)));

		//Back
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(mTiling.x, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(mTiling.x, 0)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(0, 0)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(0, mTiling.y)));

		//Top
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, 0)));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 1, 0), Vector2(0, 0)));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 1, 0), Vector2(0, mTiling.y)));

		//Bottom
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), GetColor(), Vector3(0, -1, 0), Vector2(mTiling.x, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), GetColor(), Vector3(0, -1, 0), Vector2(mTiling.x, 0)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), GetColor(), Vector3(0, -1, 0), Vector2(0, 0)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), GetColor(), Vector3(0, -1, 0), Vector2(0, mTiling.y)));

		//Left
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(mTiling.x, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(mTiling.x, 0)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(0, 0)));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(0, mTiling.y)));

		//Right
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), GetColor(), Vector3(1, 0, 0), Vector2(mTiling.x, mTiling.y)));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), GetColor(), Vector3(1, 0, 0), Vector2(mTiling.x, 0)));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), GetColor(), Vector3(1, 0, 0), Vector2(0, 0)));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), GetColor(), Vector3(1, 0, 0), Vector2(0, mTiling.y)));

		//Front
		mMesh->AddIndex(0);
		mMesh->AddIndex(1);
		mMesh->AddIndex(3);
		mMesh->AddIndex(1);
		mMesh->AddIndex(2);
		mMesh->AddIndex(3);

		//Back
		mMesh->AddIndex(7);
		mMesh->AddIndex(5);
		mMesh->AddIndex(4);
		mMesh->AddIndex(7);
		mMesh->AddIndex(6);
		mMesh->AddIndex(5);

		//Top
		mMesh->AddIndex(8);
		mMesh->AddIndex(9);
		mMesh->AddIndex(11);
		mMesh->AddIndex(9);
		mMesh->AddIndex(10);
		mMesh->AddIndex(11);

		//Bottom
		mMesh->AddIndex(15);
		mMesh->AddIndex(13);
		mMesh->AddIndex(12);
		mMesh->AddIndex(15);
		mMesh->AddIndex(14);
		mMesh->AddIndex(13);

		//Left
		mMesh->AddIndex(16);
		mMesh->AddIndex(17);
		mMesh->AddIndex(19);
		mMesh->AddIndex(17);
		mMesh->AddIndex(18);
		mMesh->AddIndex(19);

		//Right
		mMesh->AddIndex(23);
		mMesh->AddIndex(21);
		mMesh->AddIndex(20);
		mMesh->AddIndex(23);
		mMesh->AddIndex(22);
		mMesh->AddIndex(21);

		mMesh->Create();
		AddMesh(mMesh);
	}

	void Cube::Create(const Matrix4& transformationMatrix)
	{
		//Front
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(mTiling.x, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(mTiling.x, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(0, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 0, -1), Vector2(0, mTiling.y), transformationMatrix));

		//Back
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(mTiling.x, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(mTiling.x, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(0, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 0, 1), Vector2(0, mTiling.y), transformationMatrix));

		//Top
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 1, 0), Vector2(mTiling.x, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), GetColor(), Vector3(0, 1, 0), Vector2(0, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), GetColor(), Vector3(0, 1, 0), Vector2(0, mTiling.y), transformationMatrix));

		//Bottom
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), GetColor(), Vector3(0, -1, 0), Vector2(mTiling.x, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), GetColor(), Vector3(0, -1, 0), Vector2(mTiling.x, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), GetColor(), Vector3(0, -1, 0), Vector2(0, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), GetColor(), Vector3(0, -1, 0), Vector2(0, mTiling.y), transformationMatrix));

		//Left
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(mTiling.x, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(mTiling.x, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(0, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), GetColor(), Vector3(-1, 0, 0), Vector2(0, mTiling.y), transformationMatrix));

		//Right
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), GetColor(), Vector3(1, 0, 0), Vector2(mTiling.x, mTiling.y), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), GetColor(), Vector3(1, 0, 0), Vector2(mTiling.x, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), GetColor(), Vector3(1, 0, 0), Vector2(0, 0), transformationMatrix));
		mMesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), GetColor(), Vector3(1, 0, 0), Vector2(0, mTiling.y), transformationMatrix));

		//Front
		mMesh->AddIndex(0);
		mMesh->AddIndex(1);
		mMesh->AddIndex(3);
		mMesh->AddIndex(1);
		mMesh->AddIndex(2);
		mMesh->AddIndex(3);

		//Back
		mMesh->AddIndex(7);
		mMesh->AddIndex(5);
		mMesh->AddIndex(4);
		mMesh->AddIndex(7);
		mMesh->AddIndex(6);
		mMesh->AddIndex(5);

		//Top
		mMesh->AddIndex(8);
		mMesh->AddIndex(9);
		mMesh->AddIndex(11);
		mMesh->AddIndex(9);
		mMesh->AddIndex(10);
		mMesh->AddIndex(11);

		//Bottom
		mMesh->AddIndex(15);
		mMesh->AddIndex(13);
		mMesh->AddIndex(12);
		mMesh->AddIndex(15);
		mMesh->AddIndex(14);
		mMesh->AddIndex(13);

		//Left
		mMesh->AddIndex(16);
		mMesh->AddIndex(17);
		mMesh->AddIndex(19);
		mMesh->AddIndex(17);
		mMesh->AddIndex(18);
		mMesh->AddIndex(19);

		//Right
		mMesh->AddIndex(23);
		mMesh->AddIndex(21);
		mMesh->AddIndex(20);
		mMesh->AddIndex(23);
		mMesh->AddIndex(22);
		mMesh->AddIndex(21);

		mMesh->Create();
		AddMesh(mMesh);
	}

	void Cube::Create(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		mMesh->SetVertices(vertices);
		mMesh->SetIndices(indices);

		mMesh->Create();
		AddMesh(mMesh);
	}
}