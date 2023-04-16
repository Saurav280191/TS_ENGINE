#include "tspch.h"
#include "Mesh.h"
#include "Application.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	Mesh::Mesh() :
		mStatsRegistered(false)
	{

	}

	void Mesh::AddVertex(Vertex vertex)
	{
		mVertices.push_back(vertex);
	}

	void Mesh::AddIndex(uint32_t index)
	{
		mIndices.push_back(index);
	}

	void Mesh::SetVertices(std::vector<Vertex> _vertices)
	{
		mVertices = _vertices;
	}

	void Mesh::SetIndices(std::vector<uint32_t> _indices)
	{
		mIndices = _indices;
	}

	void Mesh::Create()
	{
		mVertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&mVertices[0], mVertices.size() * sizeof(Vertex));

		vertexBuffer->SetLayout({
			{ShaderDataType::FLOAT3, "a_Position"},
			{ShaderDataType::FLOAT3, "a_Color"},
			{ShaderDataType::FLOAT2, "a_TexCoord"},
			});

		mVertexArray->AddVertexBuffer(vertexBuffer);

		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&mIndices[0], mIndices.size());
		mVertexArray->SetIndexBuffer(indexBuffer);

		mVertexArray->Unbind();
	}

	void Mesh::Draw()
	{
		RenderCommand::DrawIndexed(mVertexArray, mIndices.size());

#pragma region STATS
		TS_ENGINE::Application::Get().AddDrawCalls(1);
		TS_ENGINE::Application::Get().AddVertices(mVertices.size());
		TS_ENGINE::Application::Get().AddIndices(mIndices.size());
#pragma endregion
	}
	
	void Mesh::Destroy()
	{
		for (auto& vertexBuffer : mVertexArray->GetVertexBuffers())
		{
			vertexBuffer->Unbind();
			vertexBuffer->~VertexBuffer();
		}

		auto& indexBuffer = mVertexArray->GetIndexBuffer();

		indexBuffer->Unbind();
		indexBuffer->~IndexBuffer();

		mVertexArray->~VertexArray();


		mVertices.clear();
		mIndices.clear();
		mStatsRegistered = false;
	}

	std::vector<Vertex> Mesh::GetVertices()
	{
		return mVertices;
	}

	std::vector<uint32_t> Mesh::GetIndices()
	{
		return mIndices;
	}

	std::vector<Vertex> Mesh::GetWorldSpaceVertices(Vector3 position = Vector3(0, 0, 0), Vector3 eulerAngles = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1))
	{
		Matrix4 modelMatrix =
			glm::translate(Matrix4(1), position) *
			glm::rotate(Matrix4(1), glm::radians(eulerAngles.x), Vector3(1, 0, 0)) *
			glm::rotate(Matrix4(1), glm::radians(-eulerAngles.y), Vector3(0, 1, 0)) *
			glm::rotate(Matrix4(1), glm::radians(eulerAngles.z), Vector3(0, 0, 1)) *
			glm::scale(Matrix4(1), scale);

		std::vector<Vertex> worldSpaceVertices;

		for (const Vertex& vertex : mVertices)
		{
			Vertex transformedVertex = Vertex();
			transformedVertex.position = Vector3(modelMatrix * Vector4(vertex.position, 1));
			//transformedVertex.normal = vertex.normal;
			transformedVertex.texCoord = vertex.texCoord;

			worldSpaceVertices.push_back(transformedVertex);
		}

		return worldSpaceVertices;
	}
}