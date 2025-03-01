#pragma once
#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Material.h"

namespace TS_ENGINE {

#define MAX_BONE_INFLUENCE 4

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

	enum class DrawMode
	{
		TRIANGLE,
		LINE
	};

	enum class MeshType
	{
		STATIC,
		SKINNED,
		TERRAIN
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


	struct Vertex
	{
		Vector4 position = Vector4(0, 0, 0, 0);		// Position
		Vector4 color = Vector4(1, 1, 1, 1);		// Color
		Vector2 texCoord = Vector2(0, 0);			// UV
		Vector3 normal = Vector3(0, 0, 0);			// Normal

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

	struct SkinnedVertex : public Vertex
	{
		// Bone indices that will influence vertex
		int boneIds[MAX_BONE_INFLUENCE] = { -1, -1, -1, -1 };			// Bone Ids
		// Weights from each bone
		float weights[MAX_BONE_INFLUENCE] = { 0.0f, 0.0f, 0.0f, 0.0f };	// Bone Weights

		void ResetBoneInfoToDefault()
		{
			for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
			{
				boneIds[i] = -1;
				weights[i] = 0.0f;
			}
		}

		void SetBoneIdAndWeight(int _boneId, float _weight)
		{
			for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
			{
				if (boneIds[i] == -1)
				{
					boneIds[i] = _boneId;
					weights[i] = _weight;
					break;
				}
			}
		}
	};

	struct TerrainVertex : public Vertex
	{
		float normalizedHeight = 0.0f;  // Height value for terrain displacement
	};

	class Mesh
	{
	public:
		Mesh();
		~Mesh();

		void SetName(const std::string& name);
		void SetPrimitiveType(PrimitiveType primitiveType);
		void SetMaterial(Ref<Material> material);
		void SetIndices(std::vector<uint32_t> indices);

		void AddIndex(uint32_t index);

		/// <summary>
		/// 1. Sets draw mode(Triangle/Line)
		/// 2. Creates vertex array
		/// 3. Creates vertex buffer and sets layout for it 
		/// 4. Sets vertex buffer in created vertex array
		/// 5. Creates index buffer and sets that in vertex array
		/// </summary>
		/// <param name="drawMode"></param>
		virtual void Create(DrawMode drawMode = DrawMode::TRIANGLE) = 0;

#ifdef TS_ENGINE_EDITOR
		virtual void Render(int _nodeId, bool _enableTextures) = 0;
#else
		virtual void Render(bool _enableTextures) = 0;
#endif

		virtual void Destroy();

		const std::string& GetName() const { return mName; }
		//std::vector<Vertex> GetWorldSpaceVertices(Vector3 position, Vector3 eulerAngles, Vector3 scale);
		std::vector<uint32_t>& GetIndices() { return mIndices; }
		Ref<Material> GetMaterial() const { return mMaterial; }
		PrimitiveType GetPrimitiveType() { return mPrimitiveType; }
		MeshType GetType() { return mMeshType; }

		Ref<VertexArray> GetVertexArray();
		uint32_t GetNumIndices();

		void SetHasBoneInfluence(bool _hasBoneInfluence);
		bool HasBoneInfluence() { return mHasBoneInfluence; }

	protected:
		std::string mName;
		std::vector<uint32_t> mIndices;
		PrimitiveType mPrimitiveType;
		Ref<VertexArray> mVertexArray;
		Ref<Material> mMaterial;
		DrawMode mDrawMode;
		bool mMeshCreated;
		MeshType mMeshType;

	private:
		bool mStatsRegistered;
		bool mHasBoneInfluence;
	};

	class StaticMesh : public Mesh
	{
	public:
		StaticMesh();
		~StaticMesh();

		void Create(DrawMode drawMode = DrawMode::TRIANGLE) override;

		void Render(bool _enableTextures) override;

		void Destroy() override;

		void SetVertices(std::vector<Vertex> vertices);

		void AddVertex(Vertex vertex);

		void CloneMesh(StaticMesh* mesh);

		void CloneMesh(Ref<StaticMesh> mesh);

		std::vector<Vertex>& GetVertices() { return mVertices; }
	private:
		std::vector<Vertex> mVertices;
	};

	class SkinnedMesh : public Mesh
	{
	public:
		SkinnedMesh();
		~SkinnedMesh();

		void Create(DrawMode drawMode = DrawMode::TRIANGLE) override;

		void Render(bool _enableTextures) override;

		void Destroy() override;

		void SetVertices(std::vector<SkinnedVertex> _skinnedVertices);

		void AddVertex(SkinnedVertex _skinnedVertex);

		void CloneMesh(SkinnedMesh* _mesh);

		void CloneMesh(Ref<SkinnedMesh> _mesh);

		std::vector<SkinnedVertex>& GetSkinnedVertices() { return mVertices; }
	private:
		std::vector<SkinnedVertex> mVertices;
	};
}

