#pragma once
#include "tspch.h"
#include "Primitive/Mesh.h"

namespace TS_ENGINE {

	class Cone
	{
	public:
		Cone();
		~Cone();

		Ref<Mesh> GetMesh();

	private:
		void CreateMesh(Ref<Mesh> mesh, float radius, float height, int numSegments);		
	
	private:
		float mPI = 3.141f;

		float mRadius = 0.5f;
		float mHeight = 1.0f;
		int mNumSegments = 30;
	};
}
