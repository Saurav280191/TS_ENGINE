#pragma once
#include "Renderer/Buffer.h"

namespace TS_ENGINE {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	private:
		uint32_t mRendererID;
		BufferLayout mLayout;
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(void* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	
		virtual void SetData(const void* data, uint32_t size) override;
		
		virtual const BufferLayout& GetLayout() const override 
		{
			return mLayout;
		}
		virtual void SetLayout(const BufferLayout& layout) override
		{
			mLayout = layout;
		}
		virtual unsigned int GetRendererID() const override
		{
			return mRendererID;
		}
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	private:
		uint32_t mRendererID;
		uint32_t mCount;
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const; 
		virtual void Unbind() const;

		virtual uint32_t GetCount() const 
		{
			return mCount;
		}
	};
}

