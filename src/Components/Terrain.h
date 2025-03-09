#pragma once
#include "Primitive/Mesh.h"

namespace TS_ENGINE
{
	class Terrain
	{
	public:
		Terrain();
		Terrain(double _width, double _depth, double _gridSpacing);
		~Terrain();

		void Set(double _width, double _depth, double _gridSpacing);
		void LoadHeightMap(std::string _heightMapFilepath, double _heightScale);

		double GetHeight(double _x, double _z);

		Vector3 ComputeNormal(double _x, double _z, double _heightMap);
		Vector3 ComputeNormal(double _x, double _z);

		double FractalPerlin(Vector2 pos, int octaves, double persistence);

		double PerlinNoise(Vector2 _point);
		Vector2 RandomGradient(int x, int y);

		Ref<Mesh> GetMesh() { return mMesh; }
	private:
		double mWidth;
		double mDepth;
		double mGridSpacing;

		Ref<Mesh> mMesh;

		unsigned char* mHeightMapData;
		//Ref<Texture2D> mHeightMapTexture;
		Vector2 mHeightMapRes;
	};
}

