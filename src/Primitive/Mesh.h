#pragma once
#include <Renderer/VertexArray.h>
#include <Renderer/Buffer.h>
#include <Renderer/Material.h>

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

	struct Vertex
	{
		Vector4 position = Vector4(0, 0, 0, 0);
		Vector2 texCoord = Vector2(0, 0);
		Vector3 normal = Vector3(0, 0, -1);

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
			Vector4 tranformedVertexPos = transformationMatrix * Vector4(_position.x, _position.y, _position.z, 1);
			position = Vector4(tranformedVertexPos.x, tranformedVertexPos.y, tranformedVertexPos.z, tranformedVertexPos.w);
			texCoord = _texCoord;
			normal = _normal;
		}
	};

	struct VertexWeight
	{
		unsigned int vertexID;
		float weight;
	};

	struct Bone
	{
		std::string name;
		std::vector<VertexWeight> vertexWeights;
		Matrix4 offsetMatrix;
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
		void SetBones(std::vector<Bone> bones);

		void AddVertex(Vertex vertex);
		void AddIndex(uint32_t index);

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
		void Render(int entityID);
#else
		void Render();
#endif

		void Destroy();

		const std::string GetName() const { return mName; }
		std::vector<Vertex> GetVertices() { return mVertices; }
		std::vector<Vertex> GetWorldSpaceVertices(Vector3 position, Vector3 eulerAngles, Vector3 scale);
		std::vector<uint32_t> GetIndices() { return mIndices; }
		Ref<Material> GetMaterial() const { return mMaterial; }
		PrimitiveType GetPrimitiveType() { return mPrimitiveType; }
	private:
		std::string mName;
		PrimitiveType mPrimitiveType;
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
		std::vector<Bone> mBones;
		Ref<VertexArray> mVertexArray;
		bool mStatsRegistered;
		DrawMode mDrawMode;
		Ref<Material> mMaterial;
	};
}

