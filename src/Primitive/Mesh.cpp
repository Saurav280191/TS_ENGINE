#include "tspch.h"
#include "Mesh.h"
#include "Application.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/MaterialManager.h"

namespace TS_ENGINE {

	Mesh::Mesh() 
		: mStatsRegistered(false),
		mDrawMode(TS_ENGINE::DrawMode::TRIANGLE),
		mPrimitiveType(PrimitiveType::MODEL)
	{		
		mMaterial = CreateRef<Material>(MaterialManager::GetInstance()->GetUnlitMaterial());
	}

	Mesh::~Mesh()
	{
		mVertices.clear();
		mIndices.clear();
		mVertexArray = nullptr;
		mStatsRegistered = false;
		mDrawMode = DrawMode::TRIANGLE;
		mPrimitiveType = PrimitiveType::MODEL;
	}

	void Mesh::SetName(const std::string& name)
	{
		mName = name;
	}
	
	void Mesh::SetPrimitiveType(PrimitiveType primitiveType)
	{
		mPrimitiveType = primitiveType;
	}

	void Mesh::SetMaterial(Ref<Material> material)
	{
		mMaterial = material;
	}

	void Mesh::SetVertices(std::vector<Vertex> vertices) 
	{ 
		mVertices = vertices;
	}

	void Mesh::SetIndices(std::vector<uint32_t> indices) 
	{ 
		mIndices = indices;
	}

	void Mesh::AddVertex(Vertex vertex)
	{
		mVertices.push_back(vertex);
	}

	void Mesh::AddIndex(uint32_t index)
	{
		mIndices.push_back(index);
	}

	void Mesh::Create(DrawMode drawMode)//Default is Triangle
	{
		mDrawMode = drawMode;

		mVertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&mVertices[0], (uint32_t)mVertices.size() * sizeof(Vertex));

		vertexBuffer->SetLayout({
			{ShaderDataType::FLOAT4, "a_Position"},
			{ShaderDataType::FLOAT2, "a_TexCoord"},
			{ShaderDataType::FLOAT3, "a_Normal"}
			});

		mVertexArray->AddVertexBuffer(vertexBuffer);

		if (mDrawMode == DrawMode::TRIANGLE)
		{
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&mIndices[0], (uint32_t)mIndices.size());
			mVertexArray->SetIndexBuffer(indexBuffer);
		}

		mVertexArray->Unbind();
	}

#ifdef TS_ENGINE_EDITOR
	void Mesh::Render(int entityID)
#elif
	void Mesh::Render()
#endif // TS_ENGINE_EDITOR
	{
		//To Fragment Shader
#ifdef TS_ENGINE_EDITOR
		mMaterial->Render(entityID);
#elif
		mMaterial->Render();
#endif // TS_ENGINE_EDITOR

		if (mDrawMode == DrawMode::TRIANGLE)
			RenderCommand::DrawIndexed(mVertexArray, (uint32_t)mIndices.size());
		else if (mDrawMode == DrawMode::LINE)
			RenderCommand::DrawLines(mVertexArray, (uint32_t)mVertices.size());

#pragma region STATS
		TS_ENGINE::Application::Get().AddDrawCalls(1);
		TS_ENGINE::Application::Get().AddVertices((uint32_t)mVertices.size());
		TS_ENGINE::Application::Get().AddIndices((uint32_t)mIndices.size());
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

	void Mesh::CloneMesh(Ref<Mesh> mesh)
	{
		this->mName = mesh->mName;
		this->mVertices = mesh->GetVertices();
		this->mIndices = mesh->GetIndices();
		this->mPrimitiveType = mesh->mPrimitiveType;
		this->mDrawMode = mesh->mDrawMode;

		Create(this->mDrawMode);

		this->mMaterial->CloneMaterialProperties(mesh->GetMaterial());
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
			transformedVertex.position = modelMatrix * vertex.position;
			//transformedVertex.normal = vertex.normal;
			transformedVertex.texCoord = vertex.texCoord;

			worldSpaceVertices.push_back(transformedVertex);
		}

		return worldSpaceVertices;
	}
}