#pragma once
#include "GameObject.h"
#include <Renderer/VertexArray.h>
#include <Renderer/Buffer.h>

namespace TS_ENGINE {

	struct Material
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
		Vector3 position;
		Vector3 color;
		//Vector3 normal;
		Vector2 texCoord;

		Vertex()
		{

		}

		Vertex(Vector3 _position, Vector3 _color, Vector3 _normal, Vector2 _texCoord)
		{
			position = _position;
			color = _color;
			//normal = _normal;
			texCoord = _texCoord;
		}

		Vertex(Vector3 _position, Vector3 _color, Vector3 _normal, Vector2 _texCoord, const Matrix4& transformationMatrix)
		{
			Vector4 tranformedVertexPos = transformationMatrix * Vector4(_position.x, _position.y, _position.z, 1);
			position = Vector3(tranformedVertexPos.x, tranformedVertexPos.y, tranformedVertexPos.z);
			color = _color;
			//normal = _normal;
			texCoord = _texCoord;
		}
	};

	class Mesh
	{
	private:
		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;
		
		//unsigned int mVAO, mVBO, mEBO;		
		Ref<VertexArray> mVertexArray;

		bool mStatsRegistered;
	public:
		void AddVertex(Vertex vertex);
		void AddIndex(uint32_t index);

		void SetVertices(std::vector<Vertex> _vertices);
		void SetIndices(std::vector<uint32_t> _indices);		

		void Create();
		void Draw();
		void Destroy();

		std::vector<Vertex> GetVertices();
		std::vector<Vertex> GetWorldSpaceVertices(Vector3 position, Vector3 eulerAngles, Vector3 scale);
		std::vector<uint32_t> GetIndices();
	};
}

