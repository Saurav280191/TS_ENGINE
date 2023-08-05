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

		void Create();
		void Create(const Matrix4& transformationMatrix);
	};
}

