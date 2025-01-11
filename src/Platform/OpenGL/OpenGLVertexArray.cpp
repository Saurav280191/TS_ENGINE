#include "tspch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace TS_ENGINE {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::BOOL:
			return GL_BOOL;
		case ShaderDataType::INT:
			return GL_INT;
		case ShaderDataType::INT2:
			return GL_INT;
		case ShaderDataType::INT3:
			return GL_INT;
		case ShaderDataType::INT4:
			return GL_INT;
		case ShaderDataType::FLOAT:
			return GL_FLOAT;
		case ShaderDataType::FLOAT2:
			return GL_FLOAT;
		case ShaderDataType::FLOAT3:
			return GL_FLOAT;
		case ShaderDataType::FLOAT4:
			return GL_FLOAT;
		case ShaderDataType::MAT3:
			return GL_FLOAT;
		case ShaderDataType::MAT4:
			return GL_FLOAT;
		}

		TS_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &mRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &mRendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(mRendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		TS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(mRendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.type)
			{
			case ShaderDataType::BOOL:
			{
				glEnableVertexAttribArray(mVertexBufferIndex);

				glVertexAttribIPointer(mVertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					layout.GetStride(),
					(const void*)element.offset);

				mVertexBufferIndex++;
				break;
			}
			case ShaderDataType::INT:

			case ShaderDataType::INT2:

			case ShaderDataType::INT3:

			case ShaderDataType::INT4:
			{
				glEnableVertexAttribArray(mVertexBufferIndex);

				glVertexAttribPointer(mVertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset);

				mVertexBufferIndex++;
				break;
			}
			case ShaderDataType::FLOAT:

			case ShaderDataType::FLOAT2:

			case ShaderDataType::FLOAT3:

			case ShaderDataType::FLOAT4:
			{
				glEnableVertexAttribArray(mVertexBufferIndex);

				glVertexAttribPointer(mVertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset);

				mVertexBufferIndex++;
				break;
			}
			case ShaderDataType::MAT3:

			case ShaderDataType::MAT4:
			{
				uint8_t count = element.GetComponentCount();

				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(mVertexBufferIndex);

					glVertexAttribPointer(mVertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.type),
						element.normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.offset + sizeof(float) * count * i));

					glVertexAttribDivisor(mVertexBufferIndex, 1);
					mVertexBufferIndex++;
				}
				break;
			}
			default:
				TS_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		mVertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(mRendererID);
		indexBuffer->Bind();

		mIndexBuffer = indexBuffer;
	}
}