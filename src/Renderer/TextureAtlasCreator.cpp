#include "tspch.h"
#include "TextureAtlasCreator.h"

namespace TS_ENGINE {

	TextureAtlasCreator::TextureAtlasCreator(float atlasWidth, float atlasHeight, std::vector<Ref<Texture2D>> textures)
	{
		mAtlasWidth = atlasWidth;
		mAtlasHeight = atlasHeight;
		mTextures = textures;

		StartAtlasCreation();
	}

	TextureAtlasCreator::~TextureAtlasCreator()
	{

	}

	void TextureAtlasCreator::StartAtlasCreation()
	{
		if (mTextures.size() == 0)
		{
			TS_CORE_ERROR("No texture assigned for atlasing!");
			return;
		}

		for (auto& texture : mTextures)
		{
			TexturePair texturePair = TexturePair();
			texturePair.texID = texture->GetRendererID();
			//texturePair.texture = texture;
			texturePair.placed = false;
			texturePair.checkedPlacement = false;

			mTexturePairs.push_back(texturePair);
		}

		CreateAtlas();		
	}

	std::vector<TexturePair> TextureAtlasCreator::GetTexturePairs()
	{
		return mTexturePairs;
	}

	void TextureAtlasCreator::CreateAtlas()
	{
		TextureAtlas atlas = TextureAtlas(mAtlasWidth, mAtlasHeight, mTexturePairs);

		std::vector<TexturePair> unplacedTexturePairList = {};

		for (auto& texturePair : mTexturePairs)
		{
			texturePair.checkedPlacement = false;
			texturePair.placed = false;
			atlas.OccupySlotsForTexture(texturePair);
			atlas.UncheckAllSlots();

			if (!texturePair.placed)
				unplacedTexturePairList.push_back(texturePair);
			else
			{
				TextureStPair textureStPair = TextureStPair(texturePair.texID, texturePair.rect);
				atlas.AddTextureToTextureStPairs(textureStPair);
			}
		}

		atlas.CreateTextureAtlasTexture();
		mAtlases.push_back(atlas);
	}
}