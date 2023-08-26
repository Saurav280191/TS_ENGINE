#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE {

	class Sphere
	{
	public:
		Sphere();
		~Sphere();

		void CreateVertices();
		void CreateIndices();
		Ref<Mesh> GetMesh();

	private:
		float mPI = 3.141f;
		float mRadius = 0.01f;
		int mStackCount = 20;
		int mSectorCount = 20;

		Ref<Mesh> mMesh;
	};
}
