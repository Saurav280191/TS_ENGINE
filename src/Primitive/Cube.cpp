#include "tspch.h"
#include "Cube.h"

namespace TS_ENGINE {

	Cube::Cube()
	{
		
	}

	Cube::~Cube()
	{

	}

	Ref<Mesh> Cube::GetMesh()
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetPrimitiveType(PrimitiveType::CUBE);

		//Front
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), Vector3(0, 0, -1), Vector2(1, 1)));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), Vector3(0, 0, -1), Vector2(1, 0)));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), Vector3(0, 0, -1), Vector2(0, 0)));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), Vector3(0, 0, -1), Vector2(0, 1)));

		//Back
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), Vector3(0, 0, 1), Vector2(1, 1)));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), Vector3(0, 0, 1), Vector2(1, 0)));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), Vector3(0, 0, 1), Vector2(0, 0)));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), Vector3(0, 0, 1), Vector2(0, 1)));

		//Top
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), Vector3(0, 1, 0), Vector2(1, 1)));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), Vector3(0, 1, 0), Vector2(1, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), Vector3(0, 1, 0), Vector2(0, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), Vector3(0, 1, 0), Vector2(0, 1)));

		//Bottom
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), Vector3(0, -1, 0), Vector2(1, 1)));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), Vector3(0, -1, 0), Vector2(1, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), Vector3(0, -1, 0), Vector2(0, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), Vector3(0, -1, 0), Vector2(0, 1)));

		//Left
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), Vector3(-1, 0, 0), Vector2(1, 1)));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), Vector3(-1, 0, 0), Vector2(1, 0)));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), Vector3(-1, 0, 0), Vector2(0, 0)));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), Vector3(-1, 0, 0), Vector2(0, 1)));

		//Right
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), Vector3(1, 0, 0), Vector2(1, 1)));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), Vector3(1, 0, 0), Vector2(1, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), Vector3(1, 0, 0), Vector2(0, 0)));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), Vector3(1, 0, 0), Vector2(0, 1)));

		//Front
		mesh->AddIndex(0);
		mesh->AddIndex(1);
		mesh->AddIndex(3);
		mesh->AddIndex(1);
		mesh->AddIndex(2);
		mesh->AddIndex(3);

		//Back
		mesh->AddIndex(7);
		mesh->AddIndex(5);
		mesh->AddIndex(4);
		mesh->AddIndex(7);
		mesh->AddIndex(6);
		mesh->AddIndex(5);

		//Top
		mesh->AddIndex(8);
		mesh->AddIndex(9);
		mesh->AddIndex(11);
		mesh->AddIndex(9);
		mesh->AddIndex(10);
		mesh->AddIndex(11);

		//Bottom
		mesh->AddIndex(15);
		mesh->AddIndex(13);
		mesh->AddIndex(12);
		mesh->AddIndex(15);
		mesh->AddIndex(14);
		mesh->AddIndex(13);

		//Left
		mesh->AddIndex(16);
		mesh->AddIndex(17);
		mesh->AddIndex(19);
		mesh->AddIndex(17);
		mesh->AddIndex(18);
		mesh->AddIndex(19);

		//Right
		mesh->AddIndex(23);
		mesh->AddIndex(21);
		mesh->AddIndex(20);
		mesh->AddIndex(23);
		mesh->AddIndex(22);
		mesh->AddIndex(21);

		mesh->Create();

		return mesh;
	}

	Ref<Mesh> Cube::GetMesh(const Matrix4& transformationMatrix)
	{
		Ref<Mesh> mesh = CreateRef<Mesh>();
		mesh->SetPrimitiveType(PrimitiveType::CUBE);

		//Front
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), Vector3(0, 0, -1), Vector2(1, 1), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), Vector3(0, 0, -1), Vector2(1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), Vector3(0, 0, -1), Vector2(0, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), Vector3(0, 0, -1), Vector2(0, 1), transformationMatrix));

		//Back
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), Vector3(0, 0, 1), Vector2(1, 1), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), Vector3(0, 0, 1), Vector2(1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), Vector3(0, 0, 1), Vector2(0, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), Vector3(0, 0, 1), Vector2(0, 1), transformationMatrix));

		//Top
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), Vector3(0, 1, 0), Vector2(1, 1), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), Vector3(0, 1, 0), Vector2(1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), Vector3(0, 1, 0), Vector2(0, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), Vector3(0, 1, 0), Vector2(0, 1), transformationMatrix));

		//Bottom
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), Vector3(0, -1, 0), Vector2(1, 1), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), Vector3(0, -1, 0), Vector2(1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), Vector3(0, -1, 0), Vector2(0, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), Vector3(0, -1, 0), Vector2(0, 1), transformationMatrix));

		//Left
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, 0.5f), Vector3(-1, 0, 0), Vector2(1, 1), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, 0.5f), Vector3(-1, 0, 0), Vector2(1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, 0.5f, -0.5f), Vector3(-1, 0, 0), Vector2(0, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(-0.5, -0.5f, -0.5f), Vector3(-1, 0, 0), Vector2(0, 1), transformationMatrix));

		//Right
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, 0.5f), Vector3(1, 0, 0), Vector2(1, 1), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, 0.5f), Vector3(1, 0, 0), Vector2(1, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, 0.5f, -0.5f), Vector3(1, 0, 0), Vector2(0, 0), transformationMatrix));
		mesh->AddVertex(Vertex(Vector3(0.5, -0.5f, -0.5f), Vector3(1, 0, 0), Vector2(0, 1), transformationMatrix));

		//Front
		mesh->AddIndex(0);
		mesh->AddIndex(1);
		mesh->AddIndex(3);
		mesh->AddIndex(1);
		mesh->AddIndex(2);
		mesh->AddIndex(3);

		//Back
		mesh->AddIndex(7);
		mesh->AddIndex(5);
		mesh->AddIndex(4);
		mesh->AddIndex(7);
		mesh->AddIndex(6);
		mesh->AddIndex(5);

		//Top
		mesh->AddIndex(8);
		mesh->AddIndex(9);
		mesh->AddIndex(11);
		mesh->AddIndex(9);
		mesh->AddIndex(10);
		mesh->AddIndex(11);

		//Bottom
		mesh->AddIndex(15);
		mesh->AddIndex(13);
		mesh->AddIndex(12);
		mesh->AddIndex(15);
		mesh->AddIndex(14);
		mesh->AddIndex(13);

		//Left
		mesh->AddIndex(16);
		mesh->AddIndex(17);
		mesh->AddIndex(19);
		mesh->AddIndex(17);
		mesh->AddIndex(18);
		mesh->AddIndex(19);

		//Right
		mesh->AddIndex(23);
		mesh->AddIndex(21);
		mesh->AddIndex(20);
		mesh->AddIndex(23);
		mesh->AddIndex(22);
		mesh->AddIndex(21);

		mesh->Create();
		
		return mesh;
	}
}