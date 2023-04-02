#pragma once
#include "Renderer/UniformBuffer.h"

namespace TS_ENGINE {

	class OpenGLUniformBuffer : public UniformBuffer
	{
	private:
		uint32_t mRendererID = 0;
	public:
		OpenGLUniformBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	};
}
