#pragma once
#include "Renderer/VertexArray.h"

namespace TS_ENGINE {

	class OpenGLVertexArray : public VertexArray
	{
	private:
		uint32_t mRendererID;
		uint32_t mVertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
		Ref<IndexBuffer> mIndexBuffer;
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const
		{
			return mVertexBuffers;
		}

		virtual const Ref<IndexBuffer>& GetIndexBuffer() const
		{
			return mIndexBuffer;
		}

		virtual unsigned int GetRendererID() const
		{
			return mRendererID;
		}
	};
}

