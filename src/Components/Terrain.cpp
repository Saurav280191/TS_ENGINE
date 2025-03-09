#include "tspch.h"
#include "Core/Application.h"
#include "Terrain.h"
#include "Renderer/Texture.h"

#include <stb_image.h>

namespace TS_ENGINE
{
	Terrain::Terrain() :
		mWidth(10.0),
		mDepth(10.0),
		mGridSpacing(1.0),
		mMesh(CreateRef<Mesh>()),
		mHeightMapData(nullptr),
		mHeightMapRes(0, 0)
	{
		// Set Vertices
		for (double z = 0; z < mDepth; z += mGridSpacing)
		{
			for (double x = 0; x < mWidth; x += mGridSpacing)
			{
				Vertex vertex;
				vertex.position = Vector4(x, 0, z, 1);
				vertex.normal = Vector3(0, 1, 0);
				vertex.texCoord = Vector2(static_cast<float>(x) / static_cast<float>(mWidth), static_cast<float>(z) / static_cast<float>(mDepth));

				mMesh->AddVertex(vertex);
			}
		}

		// Set Indices
		std::vector<uint32_t> indices;
		for (double z = 0; z < mDepth - mGridSpacing; z += mGridSpacing)
		{
			for (double x = 0; x < mWidth - mGridSpacing; x += mGridSpacing)
			{
				double v0 = (z / mGridSpacing) * (mWidth / mGridSpacing) + (x / mGridSpacing);
				double v1 = v0 + 1.0;
				double v2 = v0 + (mWidth / mGridSpacing);
				double v3 = v2 + 1.0;

				indices.push_back(static_cast<uint32_t>(v0));
				indices.push_back(static_cast<uint32_t>(v2));
				indices.push_back(static_cast<uint32_t>(v1));

				indices.push_back(static_cast<uint32_t>(v1));
				indices.push_back(static_cast<uint32_t>(v2));
				indices.push_back(static_cast<uint32_t>(v3));
			}
		}
		mMesh->SetIndices(indices);

		// Create Mesh
		mMesh->Create();
	}

	Terrain::Terrain(double _width, double _depth, double _gridSpacing)
	{
		TS_CORE_INFO("Creating terrain!");

		mWidth = _width;
		mDepth = _depth;
		mGridSpacing = _gridSpacing;

		mMesh = CreateRef<Mesh>();

		mHeightMapData = nullptr;
		mHeightMapRes = Vector2(0, 0);

		// Set Vertices
		for (double z = 0; z < mDepth; z += mGridSpacing)
		{
			for (double x = 0; x < mWidth; x += mGridSpacing)
			{
				Vertex vertex;
				//double height = (PerlinNoise(Vector2(x, z)) + 1.0) * 0.5;
				double height = 0.0;// FractalPerlin(Vector2(x, z), 6, 0.5) * 10.0; // Scale height
				vertex.position = Vector4(x, height, z, 1);
				vertex.normal = ComputeNormal(x, z);
				float uvScale = 0.1f; // Adjust for more/less tiling
				vertex.texCoord = glm::vec2(
					(x / mGridSpacing) * uvScale,
					(z / mGridSpacing) * uvScale
				);

				mMesh->AddVertex(vertex);
			}
		}

		// Set Indices
		std::vector<uint32_t> indices;
		
		for (double z = 0; z < mDepth - mGridSpacing; z += mGridSpacing)
		{
			for (double x = 0; x < mWidth - mGridSpacing; x += mGridSpacing)
			{
				double v0 = (z / mGridSpacing) * (mWidth / mGridSpacing) + (x / mGridSpacing);
				double v1 = v0 + 1;
				double v2 = v0 + (mWidth / mGridSpacing);
				double v3 = v2 + 1;

				indices.push_back(static_cast<uint32_t>(v0));
				indices.push_back(static_cast<uint32_t>(v2));
				indices.push_back(static_cast<uint32_t>(v1));

				indices.push_back(static_cast<uint32_t>(v1));
				indices.push_back(static_cast<uint32_t>(v2));
				indices.push_back(static_cast<uint32_t>(v3));
			}
		}

		mMesh->SetIndices(indices);

		// Create Mesh
		mMesh->Create();

		Ref<Texture2D> terrainTexture = TS_ENGINE::Texture2D::Create(Application::s_AssetsDir.string() + "\\Textures\\Terrain.png");
		//Ref<Texture2D> terrainTexture = TS_ENGINE::Texture2D::Create(Application::s_AssetsDir.string() + "\\Textures\\Seamless desert sand 00.jpg");
		//Ref<Texture2D> terrainTexture = TS_ENGINE::Texture2D::Create(Application::s_AssetsDir.string() + "\\Textures\\ground_seamless_texture_7135.jpg");
		mMesh->GetMaterial()->SetDiffuseMap(terrainTexture);
	}

	Terrain::~Terrain()
	{
		mWidth = 10.0;
		mDepth = 10.0;
		mGridSpacing = 1.0;
	}

	void Terrain::Set(double _width, double _depth, double _gridSpacing)
	{
		mWidth = _width;
		mDepth = _depth;
		mGridSpacing = _gridSpacing;
	}

	void Terrain::LoadHeightMap(std::string _heightMapFilepath, double _heightScale)
	{
		//mHeightMapTexture = Texture2D::Create(_heightMapFilepath);
		//TS_CORE_ASSERT(mHeightMapTexture->GetPixels().data(), "Failed to load heightMap!");

		// Load heightmap
		int heightMapWidth, heightMapHeight, channels;
		unsigned char* heightmapData = stbi_load(_heightMapFilepath.c_str(), &heightMapWidth, &heightMapHeight, &channels, 1);

		mHeightMapData = heightmapData;
		mHeightMapRes = Vector2(heightMapWidth, heightMapHeight);

		TS_CORE_ASSERT(mHeightMapData, "Failed to load heightMap!");
		
		mMesh = CreateRef<Mesh>();

		for (double z = 0; z < mDepth; z += mGridSpacing)
		{
			for (double x = 0; x < mWidth; x += mGridSpacing)
			{
				Vertex vertex;
				double height = GetHeight(x, z);
				vertex.position = Vector4(x, height * _heightScale, z, 1);
				//TS_CORE_INFO("Vertex: {0}, {1}, {2}", vertex.position.x, vertex.position.y, vertex.position.z);
				vertex.normal = ComputeNormal(x, z, _heightScale);
				vertex.texCoord = Vector2(x / mWidth, z / mDepth);
				vertex.height = height;

				// **Height-based coloring**
				//double normalizedHeight = height; // [0,1]
				
				Vector3 color;
				if (height < 0.3) // Low altitude (green)
					color = Vector3(0.0f, 0.6f + height * 1.3f, 0.0f);
				else if (height < 0.6) // Mid altitude (brown/gray)
					color = Vector3(0.5f, 0.4f, 0.3f);
				else if (height < 0.8) // High altitude (dark gray)
					color = Vector3(0.6f, 0.6f, 0.6f);
				else // Snowy peaks (white)
					color = Vector3(1.0f, 1.0f, 1.0f);

				vertex.color = color;

				mMesh->AddVertex(vertex);
			}
		}

		// Set Indices
		std::vector<uint32_t> indices;
		for (double z = 0; z < mDepth - mGridSpacing; z += mGridSpacing)
		{
			for (double x = 0; x < mWidth - mGridSpacing; x += mGridSpacing)
			{
				double v0 = (z / mGridSpacing) * (mWidth / mGridSpacing) + (x / mGridSpacing);
				double v1 = v0 + 1;
				double v2 = v0 + (mWidth / mGridSpacing);
				double v3 = v2 + 1;

				indices.push_back(static_cast<uint32_t>(v0));
				indices.push_back(static_cast<uint32_t>(v2));
				indices.push_back(static_cast<uint32_t>(v1));

				indices.push_back(static_cast<uint32_t>(v1));
				indices.push_back(static_cast<uint32_t>(v2));
				indices.push_back(static_cast<uint32_t>(v3));
			}
		}
		mMesh->SetIndices(indices);

		// Create Mesh
		mMesh->Create();
	}

	double Terrain::GetHeight(double _x, double _z)
	{
		if (!mHeightMapData) // No heightmap? Default to flat terrain.
			return 0.0f;

		// Scale terrain coordinates to heightmap resolution
		float imgX = (_x / mWidth) * mHeightMapRes.x;
		float imgZ = (_z / mDepth) * mHeightMapRes.y;

		int x0 = static_cast<int>(std::floor(imgX));
		int z0 = static_cast<int>(std::floor(imgZ));
		int x1 = std::min(x0 + 1, static_cast<int>(mHeightMapRes.x) - 1); // Clamp to max width
		int z1 = std::min(z0 + 1, static_cast<int>(mHeightMapRes.y) - 1); // Clamp to max height

		// Get the height values of the four surrounding points
		float h00 = mHeightMapData[z0 * static_cast<int>(mHeightMapRes.x) + x0] / 255.0f; // Top-left
		float h01 = mHeightMapData[z0 * static_cast<int>(mHeightMapRes.x) + x1] / 255.0f; // Top-right
		float h10 = mHeightMapData[z1 * static_cast<int>(mHeightMapRes.x) + x0] / 255.0f; // Bottom-left
		float h11 = mHeightMapData[z1 * static_cast<int>(mHeightMapRes.x) + x1] / 255.0f; // Bottom-right

		// Compute interpolation factors
		float dx = imgX - x0;
		float dz = imgZ - z0;

		// Perform bilinear interpolation
		float top = h00 * (1 - dx) + h01 * dx;
		float bottom = h10 * (1 - dx) + h11 * dx;
		float height = top * (1 - dz) + bottom * dz;

		// Return the interpolated height, scaled to the terrain height range
		return height;
	}

	Vector3 Terrain::ComputeNormal(double _x, double _z, double _heightMap) 
	{
		double hl = GetHeight(_x - 1, _z) * _heightMap;	// Left
		double hr = GetHeight(_x + 1, _z) * _heightMap;	// Right
		double hu = GetHeight(_x, _z - 1) * _heightMap;	// Up
		double hd = GetHeight(_x, _z + 1) * _heightMap;	// Down

		Vector3 normal = Vector3(hl - hr, 2.0f, hu - hd);
		return glm::normalize(normal);
	}
	
	Vector3 Terrain::ComputeNormal(double x, double z) 
	{
		double heightL = FractalPerlin(Vector2(x - mGridSpacing, z), 6, 0.5);
		double heightR = FractalPerlin(Vector2(x + mGridSpacing, z), 6, 0.5);
		double heightD = FractalPerlin(Vector2(x, z - mGridSpacing), 6, 0.5);
		double heightU = FractalPerlin(Vector2(x, z + mGridSpacing), 6, 0.5);

		Vector3 normal = Vector3(heightL - heightR, 2.0, heightD - heightU);
		return glm::normalize(normal);
	}

	double Terrain::FractalPerlin(Vector2 pos, int octaves, double persistence) 
	{
		double total = 0.0;
		double maxValue = 0.0;
		double amplitude = 1.0;
		float frequency = 1.0f;

		for (int i = 0; i < octaves; i++) 
		{
			total += PerlinNoise(pos * frequency) * amplitude;
			maxValue += amplitude;
			amplitude *= persistence;  // Reduces each layer's impact
			frequency *= 2.0;          // Increases frequency
		}

		return (total / maxValue + 1.0) * 0.5; // Normalize to [0,1]
	}

	double Terrain::PerlinNoise(Vector2 _point)
	{
		// 1. Determine grid cell coordinates
		glm::ivec2 cell = glm::floor(_point);  // Bottom-left corner (integer coordinates)
		glm::vec2 localPos = _point - glm::vec2(cell);  // Fractional part (local position inside the cell)

		// 2. Get gradient vectors at four corners of the grid cell
		glm::vec2 grad00 = RandomGradient(cell.x, cell.y);
		glm::vec2 grad10 = RandomGradient(cell.x + 1, cell.y);
		glm::vec2 grad01 = RandomGradient(cell.x, cell.y + 1);
		glm::vec2 grad11 = RandomGradient(cell.x + 1, cell.y + 1);

		// 3. Compute distance vectors from input point to grid corners
		glm::vec2 dist00 = localPos - glm::vec2(0, 0);
		glm::vec2 dist10 = localPos - glm::vec2(1, 0);
		glm::vec2 dist01 = localPos - glm::vec2(0, 1);
		glm::vec2 dist11 = localPos - glm::vec2(1, 1);

		// 4. Compute dot products between gradient vectors and distance vectors
		float dot00 = glm::dot(grad00, dist00);
		float dot10 = glm::dot(grad10, dist10);
		float dot01 = glm::dot(grad01, dist01);
		float dot11 = glm::dot(grad11, dist11);

		// 5. Smooth interpolation weights using Perlin's fade function
		glm::vec2 fadeT = localPos * localPos * localPos * (localPos * (localPos * 6.0f - 15.0f) + 10.0f); // Smoothstep 6t^5 - 15t^4 + 10t^3

		// 6. Bilinear interpolation using mix (Lerp)
		float lerpX1 = glm::mix(dot00, dot10, fadeT.x);
		float lerpX2 = glm::mix(dot01, dot11, fadeT.x);
		float finalValue = glm::mix(lerpX1, lerpX2, fadeT.y);

		return finalValue;
	}

	// Function to generate a random unit vector
	Vector2 Terrain::RandomGradient(int _x, int _y) 
	{
		// Seed based on grid coordinates to ensure reproducibility
		int seed = _x * 374761393 + _y * 668265263;
		srand(seed);

		// Random angle in range [0, 2π]
		double angle = (double)rand() / RAND_MAX * 2.0 * glm::pi <double>();

		// Convert to unit vector
		return { cos(angle), sin(angle) };
	}
}