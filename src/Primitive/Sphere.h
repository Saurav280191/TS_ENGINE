#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE {

	class Sphere
	{
	public:
		Sphere();
		Sphere(float _radius);
		~Sphere();

		Ref<StaticMesh> GetMesh();

	private:
		void CreateVertices(Ref<StaticMesh> mesh);
		void CreateIndices(Ref<StaticMesh> mesh);

		void CreateMesh();
	
	private:
		float mPI = 3.141f;
		float mRadius = 0.5f;
		int mStackCount = 20;
		int mSectorCount = 20;

		Ref<StaticMesh> mMesh = nullptr;
	};
}
