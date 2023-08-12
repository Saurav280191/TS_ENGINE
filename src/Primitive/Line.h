#pragma once
#include "GameObject.h"
#include <glad/glad.h>

namespace TS_ENGINE
{
	class Line : public GameObject
	{
	private:
		Ref<Mesh> mMesh;
	public:
		Line(const std::string& name);
		
		void Create(const std::vector<Vector3>& points);
		//void Draw(const std::vector<Vector4>& points);
	};
}

