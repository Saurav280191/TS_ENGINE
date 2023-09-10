#include "tspch.h"
#include <Renderer/Texture.h>
#include "Platform/OpenGL/OpenGLTexture.h"

namespace TS_ENGINE {

	std::map<std::string, uint32_t> Texture2D::mTextureStrAndIdMap;
	std::map<uint32_t, Ref<TS_ENGINE::Texture2D>> Texture2D::mTextureIdAndTexture2DMap;

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		//TODO: Add support for more APIs
		return CreateRef<OpenGLTexture2D>(width, height);
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		//TODO: Add support for more APIs
		if (mTextureStrAndIdMap.find(path) != mTextureStrAndIdMap.end())
		{
			TS_CORE_INFO("Texture from path {0} already loaded", path);
			return mTextureIdAndTexture2DMap[mTextureStrAndIdMap[path]];
		}

		Ref<OpenGLTexture2D> tex2D = CreateRef<OpenGLTexture2D>(path);
		mTextureStrAndIdMap[path] = tex2D->GetRendererID();
		mTextureIdAndTexture2DMap[tex2D->GetRendererID()] = tex2D;
		return tex2D;
	}

	Ref<Texture2D> Texture2D::Create(const char* name, const unsigned char* pixelData, uint32_t len)
	{
		//TODO: Add support for more APIs
		/*if (mTextureStrAndIdMap.find(name) != mTextureStrAndIdMap.end())
		{
			TS_CORE_INFO("Texture from path {0} already loaded", name);
			return mTextureIdAndTexture2DMap[mTextureStrAndIdMap[name]];
		}*/

		Ref<OpenGLTexture2D> tex2D = CreateRef<OpenGLTexture2D>(name, pixelData, len);
		mTextureStrAndIdMap[name] = tex2D->GetRendererID();
		mTextureIdAndTexture2DMap[tex2D->GetRendererID()] = tex2D;
		return tex2D;
	}

	const Ref<Texture2D> Texture2D::GetTextureFromID(uint32_t texID)
	{
		return mTextureIdAndTexture2DMap[texID];
	}
}