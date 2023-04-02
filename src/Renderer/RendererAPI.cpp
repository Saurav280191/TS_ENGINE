#include "tspch.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace TS_ENGINE {

	RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OPENGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (sAPI)
		{
		case RendererAPI::API::NONE:
			TS_CORE_ASSERT(false, "RendererAPI::NONE is not supported!");
			return nullptr;
		case RendererAPI::API::OPENGL:
			return CreateScope<OpenGLRendererAPI>();

			//TODO: Add support for more APIs
		}

		TS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
