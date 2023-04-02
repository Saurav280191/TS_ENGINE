#pragma once
#include "tspch.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

namespace TS_ENGINE {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    
			TS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			return nullptr;
		case RendererAPI::API::OPENGL:  
			return CreateRef<OpenGLVertexBuffer>(size);
			//TODO: Add support for more APIs
		}

		TS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;

	}

	Ref<VertexBuffer> VertexBuffer::Create(void* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:
			TS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
			//TODO: Add support for more APIs
		}

		TS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:
			TS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
			//TODO: Add support for more APIs
		}

		TS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
