#pragma once
#include <Renderer/VertexArray.h>
#include <Renderer/Buffer.h>
#include <Renderer/Material.h>

namespace TS_ENGINE {

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
		Vector3 normal = Vector3(0, 0, -1);
		Vector2 texCoord = Vector2(0, 0);

		Vertex()
		{

		}

		Vertex(Vector3 _position)
		{
			position = Vector4(_position, 1);
		}

		Vertex(Vector3 _position, Vector3 _normal, Vector2 _texCoord)
		{
			position = Vector4(_position, 1);
			normal = _normal;
			texCoord = _texCoord;
		}

		Vertex(Vector3 _position, Vector3 _normal, Vector2 _texCoord, const Matrix4& transformationMatrix)
		{
			Vector4 tranformedVertexPos = transformationMatrix * Vector4(_position.x, _position.y, _position.z, 1);
			position = Vector4(tranformedVertexPos.x, tranformedVertexPos.y, tranformedVertexPos.z, tranformedVertexPos.w);
			normal = _normal;
			texCoord = _texCoord;
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
		void SetMaterial(Ref<Material> material);
		void SetVertices(std::vector<Vertex> vertices);
		void SetIndices(std::vector<uint32_t> indices);

		void AddVertex(Vertex vertex);
		void AddIndex(uint32_t index);

		/// <summary>
		/// 1. Sets draw mode(Triangle/Line)
		/// 2. Creates vertex array
		/// 3. Create vertex buffer and sets layout for it 
		/// 4. Sets vertex buffer in created vertex array
		/// 5. Creates index buffer and sets that in vertex array
		/// </summary>
		/// <param name="drawMode"></param>
		void Create(DrawMode drawMode = DrawMode::TRIANGLE);
		void Render(int entityID);
		void Destroy();

		std::vector<Vertex> GetVertices() { return mVertices; }
		std::vector<Vertex> GetWorldSpaceVertices(Vector3 position, Vector3 eulerAngles, Vector3 scale);
		std::vector<uint32_t> GetIndices() { return mIndices; }

		Ref<Material> GetMaterial() const { return mMaterial; }

	private:
		std::string mName;
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
		Ref<VertexArray> mVertexArray;
		bool mStatsRegistered;
		DrawMode mDrawMode;
		Ref<Material> mMaterial;
	};
}

