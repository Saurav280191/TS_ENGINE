#pragma once
#include "TS_ENGINE.h"
#include "SceneManager/Node.h"
#include "TextureAtlasCreator.h"
#include "Core/Base.h"

namespace TS_ENGINE {

	struct BatchedVertex
	{
		Vector3 position;//World space position
		Vector3 color;
		Vector2 texCoord;
		GLuint texID;
	};

	class Batcher
	{
	public:
		static Batcher* GetInstance();
		~Batcher();		
		
		const bool IsBatched() const;

		void CreateAtlases();
		AtlasSizeAndTextureRectPair GetAtlasAndTextureRectPair(uint32_t texID);

		void StartBatching(std::vector<Ref<TS_ENGINE::Node>> nodes);
		void DrawBatch();
	private:
		static Batcher* m_pInstance;
		bool mBatched = false;

		std::vector<BatchedVertex> mBatchedVertices;
		std::vector<uint32_t> mBatchedIndices;

		Ref<VertexArray> mVertexArray;

		Ref<TS_ENGINE::TextureAtlasCreator> mTextureAtlasCreator;
		std::vector<Ref<TS_ENGINE::Texture2D>> mAtlasTextures;
	};
}
