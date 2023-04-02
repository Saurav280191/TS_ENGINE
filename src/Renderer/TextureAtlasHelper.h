#pragma once
#include "Core/tspch.h"
#include "Texture.h"

namespace TS_ENGINE {

	struct Rect
	{
		float x = 0;
		float y = 0;
		float w = 0;
		float h = 0;

		Rect() :
			x(0),
			y(0),
			w(0.0f),
			h(0.0f)
		{

		}

		Rect(float _x, float _y, float _w, float _h)
		{
			x = _x;
			y = _y;
			w = _w;
			h = _h;
		}
	};

	struct Slot
	{
		int id;
		int x;
		int y;
		Rect rect;
		bool vacant;
		bool checkedSlot;

		void Occupy()
		{
			vacant = false;
			TS_CORE_INFO("Slot with index {0} occupied!", id);
		}

		void Checked()
		{
			checkedSlot = true;
			TS_CORE_INFO("Slot with index {0} checked!", id);
		}

		void Uncheck()
		{
			checkedSlot = false;
		}
	};

	struct TexturePair
	{
		uint32_t texID = 0;
		Rect rect;
		bool checkedPlacement = false;
		bool placed = false;

		TexturePair() :
			texID(0),
			rect(0, 0, 0, 0),
			checkedPlacement(false),
			placed(false)
		{

		}

		TexturePair(uint32_t texID, Rect rect) :
			texID(texID),
			rect(rect),
			checkedPlacement(false),
			placed(false)
		{

		}
	};

	/// <summary>
	/// Contains textureID with UV and ST texCoord
	/// </summary>
	struct TextureStPair
	{
		uint32_t texID;
		Rect uvRect;//Texcoord
		//Rect stRect;//Normalized texCoord

		TextureStPair(uint32_t _texID, Rect _uvRect):
			texID(_texID),
			uvRect(_uvRect)
		{

		}
	};
	
	class Texture2D;

	class SlotBundle
	{
	private:
		Slot currentSlot;
		std::vector<Slot> neighbourSlots;
		Ref<Texture2D> texture;
		Rect rect;//0 <-> AtlasWidth

	public:
		void AddCurrentSlot(Slot slot)
		{
			currentSlot = slot;
			TS_CORE_ERROR("Current slot is at index: {0}", slot.id);
		}
		void AddNeighbourSlot(Slot slot)
		{
			neighbourSlots.push_back(slot);
			TS_CORE_ERROR("Neighbour slot is at index: {0}", slot.id);
		}
		Slot GetCurrentSlot()
		{
			return currentSlot;
		}
		std::vector<Slot> GetNeighbourSlots()
		{
			return neighbourSlots;
		}
		void ClearNeighbourSlots()
		{
			neighbourSlots.clear();
		}

		void SetTexture(Ref<Texture2D> _texture)
		{
			texture = _texture;
		}

		const Ref<Texture2D> GetTexture() const
		{
			return texture;
		}

		//Normalized rect for texture to be placed in Atlas 
		void CalculateRectForTexture(float textureWidth, float textureHeight, float atlasWidth, float atlasHeight)
		{
			rect = Rect(currentSlot.rect.x, currentSlot.rect.y, textureWidth, textureHeight);

			TS_CORE_INFO("Rect for texture {0}, {1}, {2}, {3}", rect.x, rect.y, rect.w, rect.h);

			{
				//uint32_t width = t->GetWidth();

				//rect.width = texture->GetWidth();
				//rect.height = texture->GetHeight();

				/*for (auto& neighbour : neighbourSlots)
				{
					if (neighbour.rect.x + neighbour.rect.width > rect.width)
						rect.width = neighbour.rect.x + neighbour.rect.width;

					if (neighbour.rect.y + neighbour.rect.height > rect.height)
						rect.height = neighbour.rect.y + neighbour.rect.height;
				}*/
			}

			//normalizedRect = Rect(rect.x / atlasWidth, rect.y / atlasHeight,
				//rect.width / atlasWidth, rect.height / atlasHeight);
		}

		const Rect GetRect() const
		{
			return rect;
		}

		//const Rect GetNormalizedRect() const
		//{
		//	return normalizedRect;
		//}
	};

	/// <summary>
	/// Used to find texture and then return AtlasSize and Rect pair
	/// </summary>
	struct AtlasSizeAndTextureRectPair
	{
	private:
		Vector2 atlasSize;
		Rect rect;
	public:
		AtlasSizeAndTextureRectPair(Vector2 _textureAtlasSize, Rect _rect)
		{
			atlasSize = _textureAtlasSize;
			rect = _rect;
		}
		const Vector2 GetAtlasSize()
		{
			return atlasSize;
		}
		const Rect GetRect()
		{
			return rect;
		}
	};
}
