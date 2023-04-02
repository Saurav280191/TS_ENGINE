#include "tspch.h"
#include "TextureAtlas.h"
#include "TextureAtlasCreator.h"

namespace TS_ENGINE {
	
	TextureAtlas::TextureAtlas() :
		mAtlasWidth(0),
		mAtlasHeight(0),
		mNumHorizontalSlots(0),
		mNumVerticalSlots(0),
		mSmallestTextureWidth(0),
		mSmallestTextureHeight(0)
	{
		
	}

	TextureAtlas::TextureAtlas(float atlasWidth, float atlasHeight, std::vector<TexturePair> texturePairs)
	{
		mAtlasWidth = atlasWidth;
		mAtlasHeight = atlasHeight;
		mTexturePairs = texturePairs;

		mSmallestTextureWidth = atlasWidth;//Set smallestTextureWidth equal to textureAtlasWidth. It will be recalculated and reset later
		mSmallestTextureHeight = atlasHeight;//Set smallestTextureheight equal to textureAtlasHeight. It will be recalculated and reset later
		
		GetSmallestTextureSize();

		mNumHorizontalSlots = mAtlasWidth / mSmallestTextureWidth;
		mNumVerticalSlots = mAtlasHeight / mSmallestTextureHeight;

		//Slot creation
		for (uint32_t y = 0; y < mNumVerticalSlots; y++)
		{
			for (uint32_t x = 0; x < mNumHorizontalSlots; x++)
			{
				uint32_t id = x + y * mNumHorizontalSlots;
				Rect rect = Rect(x * mSmallestTextureWidth, y * mSmallestTextureHeight, mSmallestTextureWidth, mSmallestTextureHeight);

				Slot slot = Slot();
				slot.id = id;
				slot.x = x;
				slot.y = y;
				slot.rect = rect;
				slot.vacant = true;
				slot.checkedSlot = false;

				mAvailableSlots.push_back(slot);
			}
		}
	}

	TextureAtlas::~TextureAtlas()
	{

	}

	void TextureAtlas::GetSmallestTextureSize()
	{
		for (auto& texturePair : mTexturePairs)
		{
			if (Texture2D::GetTextureFromID(texturePair.texID)->GetWidth() < mSmallestTextureWidth)
				mSmallestTextureWidth = Texture2D::GetTextureFromID(texturePair.texID)->GetWidth();

			if (Texture2D::GetTextureFromID(texturePair.texID)->GetHeight() < mSmallestTextureHeight)
				mSmallestTextureHeight = Texture2D::GetTextureFromID(texturePair.texID)->GetWidth();
		}
	}

	/// <summary>
	/// Occupied slots of a particular texture. 
	/// Eg: If the slot size is 128x128 and texture size is 1024x1024,
	/// then the number of slots required will be 1024/128 = 8 horizontally
	///             and							  1024/128 = 8 vertically
	/// </summary>
	/// <param name="texturePair"></param>
	void TextureAtlas::OccupySlotsForTexture(TexturePair texturePair)
	{
		TS_CORE_INFO("Finding slots for texture with path {0}, width {1}, height {2} ", 
			Texture2D::GetTextureFromID(texturePair.texID)->GetPath(), 
			Texture2D::GetTextureFromID(texturePair.texID)->GetWidth(), 
			Texture2D::GetTextureFromID(texturePair.texID)->GetHeight());

		Slot* currentVacantSlotPtr = GetNextVacantSlotIndex();
		mCurrentSlotBundle.AddCurrentSlot(*currentVacantSlotPtr);
		
		if(!currentVacantSlotPtr)
			TS_CORE_ASSERT("Could not find a vacant slot");
		
		currentVacantSlotPtr = nullptr;
		delete currentVacantSlotPtr;

		mCurrentSlotBundle.GetCurrentSlot().Checked();
		
		int requiredHorizontalSlots = Texture2D::GetTextureFromID(texturePair.texID)->GetHeight() / mSmallestTextureWidth;
		int requiredVerticalSlots = Texture2D::GetTextureFromID(texturePair.texID)->GetHeight() / mSmallestTextureHeight;

		//First check texture can't be placed in Atlas condition
		if (mCurrentSlotBundle.GetCurrentSlot().rect.x + Texture2D::GetTextureFromID(texturePair.texID)->GetWidth() > mAtlasWidth &&
			mCurrentSlotBundle.GetCurrentSlot().rect.y + Texture2D::GetTextureFromID(texturePair.texID)->GetHeight() > mAtlasHeight)
		{
			TS_CORE_ERROR("Texture with path {0} can't be placed in Atlas!", Texture2D::GetTextureFromID(texturePair.texID)->GetPath());
			texturePair.checkedPlacement = true;
			texturePair.placed = false;
		}
		else
		{
			if (mCurrentSlotBundle.GetCurrentSlot().rect.x + Texture2D::GetTextureFromID(texturePair.texID)->GetWidth() <= (float)mAtlasWidth)
			{
				for (uint32_t y = 0; y < requiredVerticalSlots; y++)
				{
					for (uint32_t x = 0; x < requiredHorizontalSlots; x++)
					{
						Slot* neighbourSlotToOccupyPtr = GetSlot(mCurrentSlotBundle.GetCurrentSlot().x + x, mCurrentSlotBundle.GetCurrentSlot().y + y);
						Slot neighbourSlotToOccupy = *neighbourSlotToOccupyPtr;						
	
						if (neighbourSlotToOccupyPtr != nullptr)
						{
							if (neighbourSlotToOccupy.id != mCurrentSlotBundle.GetCurrentSlot().id)
							{
								neighbourSlotToOccupy.Checked();								
								mCurrentSlotBundle.AddNeighbourSlot(neighbourSlotToOccupy);
								
								if (!neighbourSlotToOccupy.vacant)
								{
									TS_CORE_INFO("NeighbourSlotToOccupy at index {0} is not vacant!", neighbourSlotToOccupy.id);
									mCurrentSlotBundle.ClearNeighbourSlots();
									
									OccupySlotsForTexture(texturePair);
									break;
								}								
							}
						}
						else//Out side atlas range
						{
							TS_CORE_INFO("NeighbourSlotToOccupy is outside range!");
							OccupySlotsForTexture(texturePair);
							break;
						}
					}

					if (texturePair.checkedPlacement)
					{
						TS_CORE_INFO("Placement checked for texture with path {0}", Texture2D::GetTextureFromID(texturePair.texID)->GetPath());
						return;
					}
				}

				//Very important for keeping the texture and ST rect ready for adding the pixels to atlas later
				mCurrentSlotBundle.SetTexture(Texture2D::GetTextureFromID(texturePair.texID));
				
				mCurrentSlotBundle.CalculateRectForTexture(
					Texture2D::GetTextureFromID(texturePair.texID)->GetWidth(),
					Texture2D::GetTextureFromID(texturePair.texID)->GetHeight(),
					mAtlasWidth, 
					mAtlasHeight);

				mSlotBundles.push_back(mCurrentSlotBundle);
				mSlotBundleMap[mCurrentSlotBundle.GetTexture()->GetRendererID()] = mCurrentSlotBundle;

				if (!texturePair.checkedPlacement && !texturePair.placed)
				{
					//If all the checks are done, occupy slot and the valid neighbouring slots
					mCurrentSlotBundle.GetCurrentSlot().Occupy();
					//AddToFilledSlots(mCurrentSlotBundle.GetCurrentSlot());
					
					texturePair.rect = Rect(
						mCurrentSlotBundle.GetCurrentSlot().rect.x, 
						mCurrentSlotBundle.GetCurrentSlot().rect.y,
						mCurrentSlotBundle.GetCurrentSlot().rect.w * requiredHorizontalSlots, 
						mCurrentSlotBundle.GetCurrentSlot().rect.h * requiredVerticalSlots
					);

					RemoveSlotFromAvailableSlots(mCurrentSlotBundle.GetCurrentSlot().id);

					for(auto& validNeighbouringSlot : mCurrentSlotBundle.GetNeighbourSlots())// validNeighbouringSlots)
					{
						validNeighbouringSlot.Occupy();
						//AddToFilledSlots(validNeighbouringSlot);
						RemoveSlotFromAvailableSlots(validNeighbouringSlot.id);						
					}

					TS_CORE_INFO("Found slots for texture with name {0}, width {1}, height {2}",
						Texture2D::GetTextureFromID(texturePair.texID)->GetPath(),
						Texture2D::GetTextureFromID(texturePair.texID)->GetWidth(),
						Texture2D::GetTextureFromID(texturePair.texID)->GetHeight());

					texturePair.checkedPlacement = true;
					texturePair.placed = true;
					return;
				}
			}
		}
		
	}

	void TextureAtlas::UncheckAllSlots()
	{
		for (auto slot : mAvailableSlots)
			slot.Uncheck();
	}

	void TextureAtlas::AddTextureToTextureStPairs(TextureStPair textureStPair)
	{
		mTextureStPairs.push_back(textureStPair);
	}

	Slot* TextureAtlas::GetSlot(uint32_t x, uint32_t y)
	{
		uint32_t index = x + y * mNumHorizontalSlots;

		for (auto slot : mAvailableSlots)
		{
			if (slot.id == index)
				return &slot;
		}

		return nullptr;
	}

	Slot* TextureAtlas::GetNextVacantSlotIndex()
	{
		for (auto slot : mAvailableSlots)
		{
			if (!slot.checkedSlot)
				return &slot;
		}

		return nullptr;
	}

	void TextureAtlas::RemoveSlotFromAvailableSlots(uint32_t slotIndex)
	{
		for (int i = 0; i < mAvailableSlots.size(); i++)
		{
			if (mAvailableSlots[i].id == slotIndex)
			{
				mAvailableSlots.erase(mAvailableSlots.begin() + i);
				TS_CORE_INFO("Removed slot with index {0} from availableSlots", slotIndex);
			}
		}
	}

	/// <summary>
	/// Find the texture in atlas and returns AtlasAndTexturePair for 
	/// </summary>
	/// <param name="texID"></param>
	/// <returns></returns>
	const AtlasSizeAndTextureRectPair TextureAtlas::GetAtlasSizeAndTextureRectPair(uint32_t texID) const
	{
		auto it = mSlotBundleMap.find(texID);

		if (it != mSlotBundleMap.end())
		{
			SlotBundle slotBundle = it->second;			
			return AtlasSizeAndTextureRectPair(Vector2(mAtlasWidth, mAtlasHeight), slotBundle.GetRect());
		}
		else
		{
			TS_CORE_ERROR("Could not find texture ID: {0}", texID);
		}
	}
	
	void TextureAtlas::CreateTextureAtlasTexture()
	{
		mAtlasTexture = Texture2D::Create((uint32_t)mAtlasWidth, (uint32_t)mAtlasHeight);
		GLuint texID = CreateTextureForAtlas(mSlotBundles, mAtlasWidth, mAtlasHeight);
		mAtlasTexture->OverrideTextureID(texID);
	}

	void TextureAtlas::AddTextureToAtlas(TS_ENGINE::Rect rect, int channels, 
		std::vector<unsigned char> texData, std::vector<GLubyte>& atlasData)
	{
		if (channels == 3)
		{
			std::vector<unsigned char> paddedTexData(rect.w * rect.h * 4);

			for (int row = 0; row < rect.h; row++)
			{
				for (int col = 0; col < rect.w; col++)
				{
					int srcIndex = (row * rect.w + col) * channels;
					int dstIndex = ((rect.y + row) * mAtlasWidth + (rect.x + col)) * 4;

					atlasData[dstIndex + 0] = texData[srcIndex + 0];
					atlasData[dstIndex + 1] = texData[srcIndex + 1];
					atlasData[dstIndex + 2] = texData[srcIndex + 2];
					atlasData[dstIndex + 3] = 255;//Set alpha as 255
				}
			}
		}
		else//4 Channel
		{
			for (int row = 0; row < rect.h; row++)
			{
				for (int col = 0; col < rect.w; col++)
				{
					int srcIndex = (row * rect.w + col) * channels;
					int dstIndex = ((rect.y + row) * mAtlasWidth + (rect.x + col)) * channels;

					atlasData[dstIndex + 0] = texData[srcIndex + 0];
					atlasData[dstIndex + 1] = texData[srcIndex + 1];
					atlasData[dstIndex + 2] = texData[srcIndex + 2];
					atlasData[dstIndex + 3] = texData[srcIndex + 3];
				}
			}
		}
	}

	GLuint TextureAtlas::CreateTextureForAtlas(std::vector<SlotBundle> slotBundles, float atlasWidth, float atlasHeight)
	{
		GLuint atlasTexture;
		glGenTextures(1, &atlasTexture);
		glBindTexture(GL_TEXTURE_2D, atlasTexture);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Define texture data
		std::vector<GLubyte> atlasData(atlasWidth * atlasHeight * 4);

		for (uint32_t i = 0; i < slotBundles.size(); i++)
		{
			std::vector<unsigned char> texData = slotBundles[i].GetTexture()->GetPixels();
			uint32_t channels = slotBundles[i].GetTexture()->GetChannels();
			Rect rect = slotBundles[i].GetRect();

			AddTextureToAtlas(rect, channels, texData, atlasData);
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasWidth, atlasHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasData.data());

		return atlasTexture;
	}
}
