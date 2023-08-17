#pragma once
#include "GameObject.h"
#include <Renderer/VertexArray.h>
#include <Renderer/Buffer.h>

namespace TS_ENGINE {

	struct AssimpMaterial
	{
		aiString name;
		aiColor3D ambient;
		aiColor3D diffuse;
		aiColor3D specular;
		float opacity;
		float shininess;
	};

	struct Vertex
	{
		Vector4 position;		
		Vector3 normal;
		Vector2 texCoord;

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

		void AddVertex(Vertex vertex);
		void AddIndex(uint32_t index);

		void SetVertices(std::vector<Vertex> _vertices);
		void SetIndices(std::vector<uint32_t> _indices);

		/// <summary>
		/// 1. Sets draw mode(Traingle/Line)
		/// 2. Creates vertex array
		/// 3. Create vertex buffer and sets layout for it 
		/// 4. Sets vertex buffer in created vertex array
		/// 5. Creates index buffer and sets that in vertex array
		/// </summary>
		/// <param name="drawMode"></param>
		void Create(DrawMode drawMode = DrawMode::TRIANGLE);
		void Draw();
		void Destroy();

		std::vector<Vertex> GetVertices();
		std::vector<Vertex> GetWorldSpaceVertices(Vector3 position, Vector3 eulerAngles, Vector3 scale);
		std::vector<uint32_t> GetIndices();

	private:
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
		Ref<VertexArray> mVertexArray;
		bool mStatsRegistered;

		DrawMode mDrawMode;
	};
}

