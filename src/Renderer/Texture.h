#pragma once
#include "Core/Base.h"
#include <string>
#include <map>
#include "TextureAtlasHelper.h"

namespace TS_ENGINE {

	class Texture
	{	
	public:
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetChannels() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual const std::string& GetPath() const = 0;

		virtual void SetData(unsigned char* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void SetVerticalFlip(bool flip) const = 0;		

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
		virtual unsigned char* GetPixelData() const = 0;
		virtual std::vector<unsigned char> GetPixels() const = 0;

		virtual void OverrideTextureID(uint32_t texID) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const char* name, const unsigned char* pixelData, uint32_t len); 

		static const Ref<Texture2D> GetTextureFromID(uint32_t texID);
		static const std::map<uint32_t, Ref<TS_ENGINE::Texture2D>> GetTextureIdAndTexture2DMap()
		{
			return mTextureIdAndTexture2DMap;
		}
		
		virtual const std::string& GetPath() const override
		{
			return mPath;
		}
	private:
		static std::map<std::string, uint32_t> mTextureStrAndIdMap;
		static std::map<uint32_t, Ref<TS_ENGINE::Texture2D>> mTextureIdAndTexture2DMap;
	protected:
		std::string mPath;
	};
}
