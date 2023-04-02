#include "tspch.h"
#include "UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace TS_ENGINE {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		//TODO: Add support for more APIs
		return CreateRef<OpenGLUniformBuffer>(size, binding);
	}
}