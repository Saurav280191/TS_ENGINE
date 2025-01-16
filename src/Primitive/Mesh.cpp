#include "tspch.h"
#include "Mesh.h"
#include "Application.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	Mesh::Mesh() :
		mStatsRegistered(false),
		mDrawMode(DrawMode::TRIANGLE)
	{
		mPrimitiveType = PrimitiveType::MODEL;
		std::string shaderDir = Application::s_ResourcesDir.string() + "\\Shaders\\";
		Ref<Shader> unlitShader = Shader::Create("UnlitShader", shaderDir + "Unlit.vert", shaderDir + "Unlit.frag");
		mMaterial = CreateRef<Material>("UnlitMaterial", unlitShader);
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

	void Mesh::Create(DrawMode drawMode)// Default is Triangle
	{
		mDrawMode = drawMode;

		mVertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&mVertices[0], (uint32_t)mVertices.size() * sizeof(Vertex));

		vertexBuffer->SetLayout({
			{ ShaderDataType::FLOAT4, "a_Position"},	// Position
			{ ShaderDataType::FLOAT2, "a_TexCoord"},	// UV
			{ ShaderDataType::FLOAT3, "a_Normal"},		// Normal
			{ ShaderDataType::INT4,   "a_BoneIds" },	// Bone IDs
			{ ShaderDataType::FLOAT4, "a_Weights" }		// Bone Weights
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
	void Mesh::Render(int entityID, bool _enableTextures)
#else
	void Mesh::Render(bool _enableTextures)
#endif
	{
		// Render Material
#ifdef TS_ENGINE_EDITOR
		mMaterial->Render(entityID, _enableTextures);
#else
		mMaterial->Render(_enableTextures);
#endif

		// Render Command To Draw Geometry
		if (mDrawMode == DrawMode::TRIANGLE)
			RenderCommand::DrawIndexed(mVertexArray, (uint32_t)mIndices.size());
		else if (mDrawMode == DrawMode::LINE)
			RenderCommand::DrawLines(mVertexArray, (uint32_t)mVertices.size());

		// Add DrawCalls, Vertices and Indices for Stats
		TS_ENGINE::Application::GetInstance().AddDrawCalls(1);
		TS_ENGINE::Application::GetInstance().AddVertices((uint32_t)mVertices.size());
		TS_ENGINE::Application::GetInstance().AddIndices((uint32_t)mIndices.size());
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

	void Mesh::CloneMesh(Mesh* mesh)
	{
		this->mName = mesh->mName;
		this->mVertices = mesh->GetVertices();
		this->mIndices = mesh->GetIndices();
		this->mPrimitiveType = mesh->mPrimitiveType;
		this->mDrawMode = mesh->mDrawMode;

		Create(this->mDrawMode);

		this->mMaterial->CloneMaterialProperties(mesh->GetMaterial());
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

	Ref<VertexArray> Mesh::GetVertexArray()
	{
		return mVertexArray;
	}

	uint32_t Mesh::GetNumIndices()
	{
		return (uint32_t)mIndices.size();
	}
}