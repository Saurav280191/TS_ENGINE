#include "tspch.h"
#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace TS_ENGINE {

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height):
		mWidth(width),
		mHeight(height)
	{
		mInternalFormat = GL_RGBA8;
		mDataFormat = GL_RGBA;
		
		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path):
		mPath(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		
		data = nullptr;
		{
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
			TS_CORE_INFO("Texture loaded from path: {0}", path);
		}

		if (data)
		{
			mIsLoaded = false;

			mWidth = width;
			mHeight = height;
			mChannels = channels;

			GLenum internalFormat = 0, dataFormat = 0;

			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			//Copy pixels data to mPixels vector
			mPixels.resize(width * height * channels);
			std::memcpy(mPixels.data(), data, width * height * channels);

			mInternalFormat = internalFormat;
			mDataFormat = dataFormat;

			TS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
			glTextureStorage2D(mRendererID, 1, internalFormat, mWidth, mHeight);

			glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

			TS_CORE_INFO("Width: {0}, height : {1}, channel : {2} TextureID: {3}", mWidth, mHeight, channels, mRendererID);			

			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::OpenGLTexture2D(const char* name, const unsigned char* pixelData, uint32_t len)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		data = nullptr;
		{
			data = stbi_load_from_memory(pixelData, len, &width, &height, &channels, 0);
			TS_CORE_INFO("Embedded texture loaded with name: {0}, length: {1}, width: {2}, height: {3}, channel: {4}", name, len, width, height, channels);
		}

		if (data)
		{
			mIsLoaded = false;

			mWidth = width;
			mHeight = height;
			mChannels = channels;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4)
			{
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3)
			{
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;

			}

			//Copy pixels data to mPixels vector
			mPixels.resize(width * height * 3);
			std::memcpy(mPixels.data(), data, width * height * 3);

			mInternalFormat = internalFormat;
			mDataFormat = dataFormat;

			TS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
			glTextureStorage2D(mRendererID, 1, internalFormat, mWidth, mHeight);

			glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

			TS_CORE_INFO("TextureID: {0}", mRendererID);
			
			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &mRendererID);
	}

	void OpenGLTexture2D::SetData(unsigned char* data, uint32_t size)
	{
		this->data = data;

		uint32_t bpp = mDataFormat == GL_RGBA ? 4 : 3;
		TS_CORE_ASSERT(size == mWidth * mHeight * bpp, "Data must be entire texture!");
		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);		
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, mRendererID);
	}
	
	void OpenGLTexture2D::OverrideTextureID(GLuint texID)
	{
		mRendererID = texID;
	}
}

