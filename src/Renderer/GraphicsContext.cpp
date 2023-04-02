#include "tspch.h"
#include "Renderer/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Renderer/Renderer.h"
#include "Renderer/RendererAPI.h"

namespace TS_ENGINE {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:
			TS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
			//TODO: Add support for more APIs
		}

		TS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}