#pragma once
#include "Core/Base.h"

namespace TS_ENGINE {

	enum class ShaderDataType
	{
		NONE = 0,
		BOOL,
		INT,
		INT2,
		INT3,
		INT4,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		MAT3,
		MAT4
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::BOOL:
			return 1;
		case ShaderDataType::INT:
			return 4;
		case ShaderDataType::INT2:
			return 8;
		case ShaderDataType::INT3:
			return 12;
		case ShaderDataType::INT4:
			return 16;
		case ShaderDataType::FLOAT:
			return 4;
		case ShaderDataType::FLOAT2:
			return 8;
		case ShaderDataType::FLOAT3:
			return 12;
		case ShaderDataType::FLOAT4:
			return 16;
		case ShaderDataType::MAT3:
			return 36;
		case ShaderDataType::MAT4:
			return 64;
		}

		TS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		size_t offset;
		bool normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) :
			name(name),
			type(type),
			size(ShaderDataTypeSize(type)),
			offset(0),
			normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (type)
			{
			case ShaderDataType::INT : 
				return 1;
			case ShaderDataType::INT2 : 
				return 2;
			case ShaderDataType::INT3 : 
				return 3;
			case ShaderDataType::INT4 : 
				return 4;
			case ShaderDataType::BOOL : 
				return 1;
			case ShaderDataType::FLOAT : 
				return 1;
			case ShaderDataType::FLOAT2 : 
				return 2;
			case ShaderDataType::FLOAT3 : 
				return 3;
			case ShaderDataType::FLOAT4 : 
				return 4;
			case ShaderDataType::MAT3 : 
				return 3;
			case ShaderDataType::MAT4 : 
				return 4;
			}

			TS_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	private:
		std::vector<BufferElement> mElements;
		uint32_t mStride = 0;

		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			mStride = 0;

			for (auto& element : mElements)
			{
				element.offset = offset;
				offset += element.size;
				mStride += element.size;
			}
		}
	public:
		BufferLayout()
		{

		}

		BufferLayout(std::initializer_list<BufferElement> elements) :
			mElements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const
		{
			return mStride;
		}

		const std::vector<BufferElement>& GetElements() const
		{
			return mElements;
		}

		std::vector<BufferElement>::iterator begin() 
		{
			return mElements.begin();
		}

		std::vector<BufferElement>::iterator end()
		{
			return mElements.end();
		}

		std::vector<BufferElement>::const_iterator begin() const
		{
			return mElements.begin();
		}

		std::vector<BufferElement>::const_iterator end() const
		{
			return mElements.end();
		}
	};
	
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(void* vertices, uint32_t size);

		virtual unsigned int GetRendererID() const = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
	};
}
