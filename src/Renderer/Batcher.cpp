#include "tspch.h"
#include "Batcher.h"
#include <GameObject.h>
#include <Core/Log.h>
#include "RenderCommand.h"

namespace TS_ENGINE {

	Batcher* Batcher::m_pInstance = NULL;

	Batcher* Batcher::GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new Batcher();

		return m_pInstance;

	}
	
	Batcher::~Batcher()
	{
		/*for (auto vertexBuffer : mVertexArray->GetVertexBuffers())
		{
			vertexBuffer->Unbind();
			vertexBuffer->~VertexBuffer();
		}

		mVertexArray->GetIndexBuffer()->Unbind();
		mVertexArray->GetIndexBuffer()->~IndexBuffer();

		mVertexArray->~VertexArray();*/
	}

	void Batcher::CreateAtlases()
	{
		std::vector<Ref<Texture2D>> textures = {};
		
		for (auto& idAndTexturePair : Texture2D::GetTextureIdAndTexture2DMap())
			textures.push_back(idAndTexturePair.second);

		mTextureAtlasCreator = CreateRef<TextureAtlasCreator>(3840, 2160, textures);

		for (auto& textureAtlas : mTextureAtlasCreator->GetTextureAtlases())
		{
			auto atlasTex = textureAtlas.GetTexture();
			mAtlasTextures.push_back(atlasTex);
		}
	}
	
	AtlasSizeAndTextureRectPair Batcher::GetAtlasAndTextureRectPair(uint32_t texID)
	{
		for (auto& textureAtlas : mTextureAtlasCreator->GetTextureAtlases())
		{
			return textureAtlas.GetAtlasSizeAndTextureRectPair(texID);
		}
	}

	void Batcher::StartBatching(std::vector<Ref<TS_ENGINE::Node>> nodes)
	{
		mBatchedNode = CreateRef<TS_ENGINE::Node>();
		mBatchedNode->SetName("BatchedNode");

		Ref<GameObject> batchedGameObject = CreateRef<GameObject>();
		Ref<Mesh> batchedMesh = CreateRef<Mesh>();		

		uint32_t lastTotalVertices = 0;

		if (nodes.size() == 0)
			return;

		for (auto& node : nodes)
		{
			uint32_t meshIndex = 0;
			Ref<GameObject> go = node->GetAttachedGameObject();

			uint32_t texID = go->GetTextureID();
			//TS_CORE_INFO("Texture ID for {0} is {1}",go->GetName(), texID);
			AtlasSizeAndTextureRectPair atlasAndTextureRectPair = GetAtlasAndTextureRectPair(texID);
			
			Rect rect = atlasAndTextureRectPair.GetRect();//Rect between 0 - texWidth, 0, texHeight
			float atlasWidth = atlasAndTextureRectPair.GetAtlasSize().x;
			float atlasHeight = atlasAndTextureRectPair.GetAtlasSize().y;

			float u1 = rect.x;
			float v1 = rect.y;
			float u2 = (rect.x + rect.w);
			float v2 = (rect.y + rect.h);

			for (auto& mesh : go->GetMeshes())
			{
				auto vertices = mesh->GetWorldSpaceVertices(node->GetTransform()->GetLocalPosition(), node->GetTransform()->GetLocalEulerAngles(), node->GetTransform()->GetLocalScale());
				auto indices = mesh->GetIndices();
				GLuint currentMeshTexID = go->GetTextureID();

				//BatchedVertex batchedVertex;
				Vertex batchedVertex;

				for (const TS_ENGINE::Vertex& vertex : vertices)
				{
					batchedVertex.position = vertex.position;
					batchedVertex.color = go->GetColor();					
					
					float u = (u1 + vertex.texCoord.x * rect.w) / atlasWidth;
					float v = (v1 + vertex.texCoord.y * rect.h) / atlasHeight;

					//TS_CORE_INFO("Texcoord original : {0}, {1}", vertex.texCoord.x, vertex.texCoord.y);
					//TS_CORE_INFO("Texcoord : {0}, {1}", u, v);

					Vector2 alteredTexCoord = Vector2(u, v);
					//batchedVertex.texCoord = vertex.texCoord;
					batchedVertex.texCoord = alteredTexCoord;

					//TS_CORE_INFO("UV: {0}, {1}", vertex.uv.x, vertex.uv.y);
					//batchedVertex.texID = currentMeshTexID;
					//mBatchedVertices.push_back(batchedVertex);
					batchedMesh->AddVertex(batchedVertex);
				}

				for (const uint32_t& index : indices)
				{
					//mBatchedIndices.push_back(lastTotalVertices + index);
					batchedMesh->AddIndex(lastTotalVertices + index);
				}

				lastTotalVertices += vertices.size();

				//TS_CORE_INFO("Texture ID for {0}'s mesh at {1} is {2}",go->GetName(), meshIndex ,go->GetTextureID());
				//auto tex2D = Texture2D::GetTextureFromID(go->GetTextureID());

				meshIndex++;
			}
		}

		//mVertexArray = VertexArray::Create();
		//auto vao = mVertexArray->GetRendererID();

		//Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&mBatchedVertices[0], mBatchedVertices.size() * sizeof(Vertex));
		//auto vbo = vertexBuffer->GetRendererID();
		//
		//vertexBuffer->SetLayout({
		//	{ShaderDataType::FLOAT3, "a_Position"},
		//	{ShaderDataType::FLOAT3, "a_Color"},
		//	{ShaderDataType::FLOAT2, "a_TexCoord"}//,
		//	//{ShaderDataType::INT, "a_TexID"},
		//	});

		//mVertexArray->AddVertexBuffer(vertexBuffer);

		//Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&mBatchedIndices[0], mBatchedIndices.size());
		//
		//mVertexArray->SetIndexBuffer(indexBuffer);
		//
		//mVertexArray->Unbind();

		batchedMesh->Create();
		batchedGameObject->AddMesh(batchedMesh);
		batchedGameObject->SetTexture(mAtlasTextures[0]);
		mBatchedNode->AttachGameObject(batchedGameObject);

		TS_CORE_INFO("***Batching completed***");
		mBatched = true;
	}

	//void Batcher::DrawBatch()
	//{
		/*mAtlasTextures[0]->Bind();
		RenderCommand::DrawIndexed(mVertexArray, mBatchedIndices.size());
		
		TS_ENGINE::Application::Get().AddDrawCalls(1);
		TS_ENGINE::Application::Get().AddVertices(mBatchedVertices.size());
		TS_ENGINE::Application::Get().AddIndices(mBatchedIndices.size());*/
	//}
}