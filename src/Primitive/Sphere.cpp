#include "tspch.h"
#include "Sphere.h"

namespace TS_ENGINE {

	Sphere::Sphere()
	{
		CreateMesh();
	}

	Sphere::~Sphere()
	{

	}

	void Sphere::CreateVertices(Ref<Mesh> mesh)
	{
		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / mRadius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * mPI / mSectorCount;
		float stackStep = mPI / mStackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= mStackCount; ++i)
		{
			stackAngle = mPI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = mRadius * cosf(stackAngle);             // r * cos(u)
			z = mRadius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= mSectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

				// vertex tex coord (s, t) range between [0, 1]
				s = (float)j / mSectorCount;
				t = (float)i / mStackCount;
				
				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;

				Vertex vertex = Vertex(Vector3(x, y, z), Vector2(s, t), Vector3(nx, ny, nz));
				mesh->AddVertex(vertex);
			}
		}
	}

	void Sphere::CreateIndices(Ref<Mesh> mesh)
	{
		// generate CCW index list of sphere triangles
		// k1--k1+1
		// |  / |
		// | /  |
		// k2--k2+1
		std::vector<int> lineIndices;
		int k1, k2;
		for (int i = 0; i < mStackCount; ++i)
		{
			k1 = i * (mSectorCount + 1);     // beginning of current stack
			k2 = k1 + mSectorCount + 1;      // beginning of next stack

			for (int j = 0; j < mSectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					mesh->AddIndex(k1);
					mesh->AddIndex(k2);
					mesh->AddIndex(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (mStackCount - 1))
				{
					mesh->AddIndex(k1 + 1);
					mesh->AddIndex(k2);
					mesh->AddIndex(k2 + 1);
				}

				// store indices for lines
				// vertical lines for all stacks, k1 => k2
				lineIndices.push_back(k1);
				lineIndices.push_back(k2);

				if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
				{
					lineIndices.push_back(k1);
					lineIndices.push_back(k1 + 1);
				}
			}
		}
	}

	void Sphere::CreateMesh()
	{
		mMesh = CreateRef<Mesh>();
		mMesh->SetPrimitiveType(PrimitiveType::SPHERE);

		CreateVertices(mMesh);
		CreateIndices(mMesh);

		mMesh->Create();
	}

	Ref<Mesh> Sphere::GetMesh()
	{
		return mMesh;
	}
}