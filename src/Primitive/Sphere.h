#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE {

	class Sphere
	{
	public:
		Sphere();
		~Sphere();

		Ref<Mesh> GetMesh();

	private:
		void CreateVertices(Ref<Mesh> mesh);
		void CreateIndices(Ref<Mesh> mesh);

		void CreateMesh();
	
	private:
		float mPI = 3.141f;
		float mRadius = 0.5f;
		int mStackCount = 20;
		int mSectorCount = 20;

		Ref<Mesh> mMesh = nullptr;
	};
}
