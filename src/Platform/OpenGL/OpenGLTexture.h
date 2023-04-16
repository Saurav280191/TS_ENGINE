#pragma once
#include "Renderer/Texture.h"
#include <glad/glad.h>
#include <Renderer/TextureAtlasHelper.h>

namespace TS_ENGINE {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const char* name, const unsigned char* pixelData, uint32_t len);

		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override 
		{ 
			return mWidth; 
		}
		virtual uint32_t GetHeight() const override 
		{ 
			return mHeight; 
		}
		virtual uint32_t GetChannels() const override
		{
			return mChannels;
		}
		virtual uint32_t GetRendererID() const override 
		{ 
			return mRendererID;
		}

		virtual const std::string& GetPath() const override
		{
			return mPath;
		}

		virtual void SetData(unsigned char* data, uint32_t size) override;
		virtual void Bind(uint32_t slot) const override;
		virtual void SetVerticalFlip(bool flip) const override;
		
		virtual bool IsLoaded() const override
		{
			return mIsLoaded;
		}
		virtual bool operator==(const Texture& other) const override
		{
			return mRendererID == other.GetRendererID();
		}

		virtual unsigned char* GetPixelData() const override
		{
			return data;
		}
		virtual std::vector<unsigned char> GetPixels() const override
		{
			return mPixels;
		}
		
		virtual void OverrideTextureID(GLuint texID) override;
	private:
		unsigned char* data = nullptr;
		std::vector<unsigned char> mPixels;
		std::string mPath;
		bool mIsLoaded = false;
		uint32_t mWidth, mHeight, mChannels;
		uint32_t mRendererID;
		GLenum mInternalFormat, mDataFormat;
	};
}