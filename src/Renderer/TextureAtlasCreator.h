#pragma once
#include "Core/tspch.h"
#include "TS_ENGINE.h"
#include "Core/Base.h"
#include "Texture.h"
#include "TextureAtlasHelper.h"
#include "TextureAtlas.h"

namespace TS_ENGINE {

	class TextureAtlasCreator
	{
	public:
		TextureAtlasCreator(float atlasWidth, float atlasHeight, std::vector<Ref<Texture2D>> textures);
		~TextureAtlasCreator();

		void StartAtlasCreation();
		
		std::vector<TexturePair> GetTexturePairs();
		const std::vector<TextureAtlas> GetTextureAtlases() const
		{
			return mAtlases;
		}
	private:
		float mAtlasWidth;
		float mAtlasHeight;
		std::vector<Ref<Texture2D>> mTextures;
		std::vector<TexturePair> mTexturePairs;
		std::vector<TextureAtlas> mAtlases;
		void CreateAtlas();
	};
}
