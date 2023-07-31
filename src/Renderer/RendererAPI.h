#pragma once
#include "VertexArray.h"
#include <GLM/glm.hpp>

namespace TS_ENGINE {

	class RendererAPI
	{
	public:
		enum class API
		{
			NONE = 0,
			OPENGL = 1
			//TODO: Add support for more APIs
		};
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;

		virtual void SetLineWidth(float width) = 0;
		
		virtual void EnableDepthTest(bool enable) = 0;

		static API GetAPI() 
		{
			return sAPI;
		}

		static Scope<RendererAPI> Create();
	private:
		static API sAPI;
	};
}