#pragma once
#include "GameObject.h"

namespace TS_ENGINE {

	class Quad : public GameObject
	{
	private:
		Ref<Mesh> mMesh;
	public:
		Quad(const std::string& name);
		Quad(const std::string& name, const Vector3& color);
		
		void OverrideTexCoords(std::vector<Vector2> texCoords);

		/// <summary>
		/// Creates mesh data and vertexBuffer using mesh::Create() func
		/// And add the mesh in GameObject's mMeshes vector
		/// </summary>
		void Create();
		void Create(const Matrix4& transformationMatrix);
	};
}

