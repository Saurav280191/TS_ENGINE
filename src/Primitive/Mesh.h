#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Material.h"

namespace TS_ENGINE {

	enum class PrimitiveType
	{
		LINE,
		QUAD,
		CUBE,
		SPHERE,
		CYLINDER,
		CONE,
		MODEL
	};

	struct AssimpMaterial
	{
		aiString name;
		aiColor3D ambient = aiColor3D(1, 1, 1);
		aiColor3D diffuse = aiColor3D(1, 1, 1);
		aiColor3D specular = aiColor3D(1, 1, 1);
		float opacity = 0.0f;
		float shininess = 0.0f;
	};

#define MAX_BONE_INFLUENCE 4

	struct Vertex
	{
		Vector4 position = Vector4(0, 0, 0, 0);		// Position
		Vector2 texCoord = Vector2(0, 0);			// UV
		Vector3 normal = Vector3(0, 0, 0);			// Normal

		// Bone indices that will influence vertex
		int mBoneIds[MAX_BONE_INFLUENCE] = { -1, -1, -1, -1 };			// Bone Ids
		// Weights from each bone
		float mWeights[MAX_BONE_INFLUENCE] = {0.0f, 0.0f, 0.0f, 0.0f };	// Bone Weights

		//Vector3 tangent;							// Tangent
		//Vector3 biTangent;						// Bi-Tangent

		Vertex()
		{

		}

		Vertex(Vector3 _position)
		{
			position = Vector4(_position, 1);
		}

		Vertex(Vector3 _position, Vector2 _texCoord, Vector3 _normal)
		{
			position = Vector4(_position, 1);
			texCoord = _texCoord;
			normal = _normal;
		}

		Vertex(Vector3 _position, Vector2 _texCoord, Vector3 _normal, const Matrix4& transformationMatrix)
		{
			Vector4 transformedVertexPos = transformationMatrix * Vector4(_position.x, _position.y, _position.z, 1);
			position = Vector4(transformedVertexPos.x, transformedVertexPos.y, transformedVertexPos.z, transformedVertexPos.w);
			texCoord = _texCoord;
			normal = _normal;
		}
	};

	enum DrawMode
	{
		TRIANGLE,
		LINE
	};

	class Mesh
	{
	public:
		Mesh();		
		~Mesh();

		void SetName(const std::string& name);
		void SetPrimitiveType(PrimitiveType primitiveType);
		void SetMaterial(Ref<Material> material);
		void SetVertices(std::vector<Vertex> vertices);
		void SetIndices(std::vector<uint32_t> indices);		

		void AddVertex(Vertex vertex);
		void AddIndex(uint32_t index);

		void CloneMesh(Mesh* mesh);
		void CloneMesh(Ref<Mesh> mesh);

		/// <summary>
		/// 1. Sets draw mode(Triangle/Line)
		/// 2. Creates vertex array
		/// 3. Creates vertex buffer and sets layout for it 
		/// 4. Sets vertex buffer in created vertex array
		/// 5. Creates index buffer and sets that in vertex array
		/// </summary>
		/// <param name="drawMode"></param>
		void Create(DrawMode drawMode = DrawMode::TRIANGLE);

#ifdef TS_ENGINE_EDITOR
		void Render(int entityID, bool _enableTextures);
#else
		void Render(bool _enableTextures);
#endif

		void Destroy();

		const std::string GetName() const { return mName; }
		std::vector<Vertex>& GetVertices() { return mVertices; }
		std::vector<Vertex> GetWorldSpaceVertices(Vector3 position, Vector3 eulerAngles, Vector3 scale);
		std::vector<uint32_t>& GetIndices() { return mIndices; }
		Ref<Material> GetMaterial() const { return mMaterial; }
		PrimitiveType GetPrimitiveType() { return mPrimitiveType; }

		Ref<VertexArray> GetVertexArray();
		uint32_t GetNumIndices();		
	private:
		std::string mName;
		PrimitiveType mPrimitiveType;
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;		
		Ref<VertexArray> mVertexArray;
		bool mStatsRegistered;
		DrawMode mDrawMode;
		Ref<Material> mMaterial;		
	};
}

