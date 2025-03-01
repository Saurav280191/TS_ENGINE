#include "tspch.h"
#include "Mesh.h"
#include "Application.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/MaterialManager.h"

namespace TS_ENGINE {

	Mesh::Mesh() :
		mStatsRegistered(false),
		mDrawMode(DrawMode::TRIANGLE),
		mHasBoneInfluence(false),
		mMeshCreated(false),
		mMeshType(MeshType::STATIC)
	{
		mPrimitiveType = PrimitiveType::MODEL;
		std::string shaderDir = Application::s_ResourcesDir.string() + "\\Shaders\\";
		//Ref<Shader> unlitShader = Shader::Create("UnlitShader", shaderDir + "Unlit.vert", shaderDir + "Unlit.frag");
		//mMaterial = CreateRef<Material>("UnlitMaterial", unlitShader);
		mMaterial = CreateRef<Material>(*MaterialManager::GetInstance()->GetMaterial("Unlit"));
	}

	Mesh::~Mesh()
	{
		TS_CORE_INFO("Destroying mesh named: {0}", mName);

		mIndices.clear();
		mVertexArray = nullptr;
		mStatsRegistered = false;
		mDrawMode = DrawMode::TRIANGLE;
		mPrimitiveType = PrimitiveType::MODEL;
		mHasBoneInfluence = false;
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

	void Mesh::SetIndices(std::vector<uint32_t> indices)
	{
		mIndices = indices;
	}

	void Mesh::AddIndex(uint32_t index)
	{
		mIndices.push_back(index);
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

		mIndices.clear();
		mStatsRegistered = false;
	}

	//std::vector<Vertex> Mesh::GetWorldSpaceVertices(Vector3 position = Vector3(0, 0, 0), Vector3 eulerAngles = Vector3(0, 0, 0), Vector3 scale = Vector3(1, 1, 1))
	//{
	//	Matrix4 modelMatrix =
	//		glm::translate(Matrix4(1), position) *
	//		glm::rotate(Matrix4(1), glm::radians(eulerAngles.x), Vector3(1, 0, 0)) *
	//		glm::rotate(Matrix4(1), glm::radians(-eulerAngles.y), Vector3(0, 1, 0)) *
	//		glm::rotate(Matrix4(1), glm::radians(eulerAngles.z), Vector3(0, 0, 1)) *
	//		glm::scale(Matrix4(1), scale);

	//	std::vector<Vertex> worldSpaceVertices;

	//	for (const Vertex& vertex : mVertices)
	//	{
	//		Vertex transformedVertex = Vertex();
	//		transformedVertex.position = modelMatrix * vertex.position;
	//		//transformedVertex.normal = vertex.normal;
	//		transformedVertex.texCoord = vertex.texCoord;

	//		worldSpaceVertices.push_back(transformedVertex);
	//	}

	//	return worldSpaceVertices;
	//}

	Ref<VertexArray> Mesh::GetVertexArray()
	{
		return mVertexArray;
	}

	uint32_t Mesh::GetNumIndices()
	{
		return (uint32_t)mIndices.size();
	}

	void Mesh::SetHasBoneInfluence(bool _hasBoneInfluence)
	{
		mHasBoneInfluence = _hasBoneInfluence;
	}

#pragma region StaticMesh Functions

	StaticMesh::StaticMesh()
	{
		mMeshType = MeshType::STATIC;
	}

	StaticMesh::~StaticMesh()
	{
		mVertices.clear();
	}

	void StaticMesh::Create(DrawMode drawMode)
	{
		if (mVertices.size() > 0 && mIndices.size() == 0
			|| mIndices.size() > 0 && mVertices.size() == 0)
		{
			TS_CORE_ERROR("Vertices Or Indices Are Not Set Properly!");
			return;
		}

		mDrawMode = drawMode;

		mVertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&mVertices[0], (uint32_t)mVertices.size() * sizeof(Vertex));

		vertexBuffer->SetLayout({
			{ ShaderDataType::FLOAT4, "a_Position"},	// Position
			{ ShaderDataType::FLOAT4, "a_Color"},		// Color
			{ ShaderDataType::FLOAT2, "a_TexCoord"},	// UV
			{ ShaderDataType::FLOAT3, "a_Normal"},		// Normal
			});

		mVertexArray->AddVertexBuffer(vertexBuffer);

		if (mDrawMode == DrawMode::TRIANGLE)
		{
			Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&mIndices[0], (uint32_t)mIndices.size());
			mVertexArray->SetIndexBuffer(indexBuffer);
		}

		mVertexArray->Unbind();

		mMeshCreated = true;
	}

#ifdef TS_ENGINE_EDITOR
	void StaticMesh::Render(int _nodeId, bool _enableTextures)
#else
	void StaticMesh::Render(bool _enableTextures)
#endif
	{
		TS_CORE_ASSERT(mMeshCreated, "Mesh has not been created!");

		// Render Material
#ifdef TS_ENGINE_EDITOR
		mMaterial->Render(_nodeId, _enableTextures);
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

	void StaticMesh::Destroy()
	{
		mVertices.clear();
	}

	void StaticMesh::SetVertices(std::vector<Vertex> vertices)
	{
		mVertices = vertices;
	}

	void StaticMesh::AddVertex(Vertex vertex)
	{
		mVertices.push_back(vertex);
	}

	void StaticMesh::CloneMesh(StaticMesh* _mesh)
	{
		mName = _mesh->mName;					// Name
		mVertices = _mesh->mVertices;			// Vertices
		mIndices = _mesh->mIndices;				// Indices
		mPrimitiveType = _mesh->mPrimitiveType;	// PrimitiveType
		mDrawMode = _mesh->mDrawMode;			// DrawMode
		mMeshType = _mesh->mMeshType;			// MeshType

		// 1. Sets draw mode(Triangle/Line) 
		// 2. Creates vertex array 
		// 3. Creates vertex buffer and sets layout for it 
		// 4. Sets vertex buffer in created vertex array 
		// 5. Creates index buffer and sets that in vertex array
		Create(mDrawMode);

		mMaterial->CloneMaterialProperties(_mesh->GetMaterial());
	}

	void StaticMesh::CloneMesh(Ref<StaticMesh> _mesh)
	{
		mName = _mesh->mName;					// Name
		mVertices = _mesh->mVertices;			// Vertices

		mIndices = _mesh->mIndices;				// Indices
		mPrimitiveType = _mesh->mPrimitiveType;	// PrimitiveType
		mDrawMode = _mesh->mDrawMode;			// DrawMode
		mMeshType = _mesh->mMeshType;			// MeshType

		// 1. Sets draw mode(Triangle/Line) 
		// 2. Creates vertex array 
		// 3. Creates vertex buffer and sets layout for it 
		// 4. Sets vertex buffer in created vertex array 
		// 5. Creates index buffer and sets that in vertex array
		Create(mDrawMode);

		mMaterial->CloneMaterialProperties(_mesh->GetMaterial());
	}

#pragma endregion

#pragma region SkinnedMesh Functions

	SkinnedMesh::SkinnedMesh()
	{
		mMeshType = MeshType::SKINNED;
	}

	SkinnedMesh::~SkinnedMesh()
	{	
		mVertices.clear();		
	}

	void SkinnedMesh::Create(DrawMode drawMode)
	{
		if (mVertices.size() > 0 && mIndices.size() == 0
			|| mIndices.size() > 0 && mVertices.size() == 0)
		{
			TS_CORE_ERROR("Vertices Or Indices Are Not Set Properly!");
			return;
		}

		mDrawMode = drawMode;

		mVertexArray = VertexArray::Create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&mVertices[0], (uint32_t)mVertices.size() * sizeof(Vertex));

		vertexBuffer->SetLayout({
			{ ShaderDataType::FLOAT4, "a_Position"},	// Position
			{ ShaderDataType::FLOAT4, "a_Color"},		// Color
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

		mMeshCreated = true;
	}

	void SkinnedMesh::Render(bool _enableTextures)
	{
		TS_CORE_ASSERT(mMeshCreated, "Mesh has not been created!");

		// Render Material
#ifdef TS_ENGINE_EDITOR
		mMaterial->Render(_nodeId, _enableTextures);
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

	void SkinnedMesh::Destroy()
	{
		mVertices.clear();
	}

	void SkinnedMesh::SetVertices(std::vector<SkinnedVertex> _skinnedVertices)
	{
		mVertices = _skinnedVertices;
	}

	void SkinnedMesh::AddVertex(SkinnedVertex _skinnedVertex)
	{
		mVertices.push_back(_skinnedVertex);
	}

	void SkinnedMesh::CloneMesh(SkinnedMesh* _mesh)
	{
		mName = _mesh->mName;					// Name
		mVertices = _mesh->mVertices;			// Vertices
		mIndices = _mesh->mIndices;				// Indices
		mPrimitiveType = _mesh->mPrimitiveType;	// PrimitiveType
		mDrawMode = _mesh->mDrawMode;			// DrawMode
		mMeshType = _mesh->mMeshType;			// MeshType

		// 1. Sets draw mode(Triangle/Line) 
		// 2. Creates vertex array 
		// 3. Creates vertex buffer and sets layout for it 
		// 4. Sets vertex buffer in created vertex array 
		// 5. Creates index buffer and sets that in vertex array
		Create(mDrawMode);

		mMaterial->CloneMaterialProperties(_mesh->GetMaterial());
	}

	void SkinnedMesh::CloneMesh(Ref<SkinnedMesh> _mesh)
	{
		mName = _mesh->mName;					// Name
		mVertices = _mesh->mVertices;			// Vertices

		mIndices = _mesh->mIndices;				// Indices
		mPrimitiveType = _mesh->mPrimitiveType;	// PrimitiveType
		mDrawMode = _mesh->mDrawMode;			// DrawMode
		mMeshType = _mesh->mMeshType;			// MeshType

		// 1. Sets draw mode(Triangle/Line) 
		// 2. Creates vertex array 
		// 3. Creates vertex buffer and sets layout for it 
		// 4. Sets vertex buffer in created vertex array 
		// 5. Creates index buffer and sets that in vertex array
		Create(mDrawMode);

		mMaterial->CloneMaterialProperties(_mesh->GetMaterial());
	}

#pragma endregion
}