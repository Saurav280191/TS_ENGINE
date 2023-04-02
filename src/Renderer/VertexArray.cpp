#include "tspch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "RendererAPI.h"

namespace TS_ENGINE {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (RendererAPI::GetAPI())
		{
		default:
			break;
		}
		//TODO: Add support for more APIs
		return CreateRef<OpenGLVertexArray>();
	}
}