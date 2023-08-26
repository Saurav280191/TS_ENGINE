#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE {

	class Quad
	{
	public:
		Quad();
		~Quad();

		//void OverrideTexCoords(std::vector<Vector2> texCoords);

		/// <summary>
		/// Creates mesh data and vertexBuffer using mesh::Create() func
		/// And add the mesh in GameObject's mMeshes vector
		/// </summary>
		Ref<Mesh> GetMesh();
		Ref<Mesh> GetMesh(const Matrix4& transformationMatrix);
	};
}

