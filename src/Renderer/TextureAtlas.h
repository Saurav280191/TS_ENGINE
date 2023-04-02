#pragma once
#include "Core/tspch.h"
#include "TS_ENGINE.h"
#include "Texture.h"
#include "TextureAtlasHelper.h"

namespace TS_ENGINE {

	class TextureAtlas
	{
	private:
		/// <summary>
		/// Iterates through all the textures in TextureAtlasCreators's m_TexturePaits 
		/// and get's the smallest texture size. The slot size will be made accordingly
		/// </summary>
		void GetSmallestTextureSize();
		Ref<Texture2D> mAtlasTexture;
	public:
		TextureAtlas();
		TextureAtlas(float width, float height, std::vector<TexturePair> texturePairs);
		~TextureAtlas();

		void OccupySlotsForTexture(TexturePair texturePair);
		void UncheckAllSlots();
		
		void AddTextureToTextureStPairs(TextureStPair textureStPair);
		void CreateTextureAtlasTexture();

		const Ref<Texture2D> GetTexture() const
		{
			return mAtlasTexture;
		}
		const AtlasSizeAndTextureRectPair GetAtlasSizeAndTextureRectPair(uint32_t texID) const;
	private:
		Slot* GetSlot(uint32_t x, uint32_t y);
		Slot* GetNextVacantSlotIndex();
		void RemoveSlotFromAvailableSlots(uint32_t slotIndex);
		
		/// <summary>
		/// Takes texture rect, channel, texel unsigned char vector, and copies the texels to atlasData ie: GLubyte vector
		/// </summary>
		/// <param name="rect"></param>
		/// <param name="channels"></param>
		/// <param name="texData"></param>
		/// <param name="atlasWidth"></param>
		/// <param name="atlasHeight"></param>
		/// <param name="atlasData"></param>
		void AddTextureToAtlas(TS_ENGINE::Rect rect, int channels, std::vector<unsigned char> texData, std::vector<GLubyte>& atlasData);		
		GLuint CreateTextureForAtlas(std::vector<SlotBundle> slotBundles, float atlasWidth, float atlasHeight);
	private:		
		float mAtlasWidth;
		float mAtlasHeight;
		
		float mSmallestTextureWidth;
		float mSmallestTextureHeight;
		
		int mNumHorizontalSlots;
		int mNumVerticalSlots;

		/// <summary>
		/// Crucial for placing the texture pixels in Atlas
		/// </summary>
		SlotBundle mCurrentSlotBundle;

		std::vector<Slot> mAvailableSlots;
		std::vector<SlotBundle> mSlotBundles;
		std::unordered_map<uint32_t, SlotBundle> mSlotBundleMap;
		std::vector<TexturePair> mTexturePairs;
		std::vector<TextureStPair> mTextureStPairs;
	};
}