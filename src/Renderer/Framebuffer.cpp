#include "Renderer/Framebuffer.h"
#include <Platform/OpenGL/OpenGLFramebuffer.h>

namespace TS_ENGINE {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		//ToDo: Add support for multiple APIs
		return CreateRef<OpenGLFramebuffer>(spec);
	}
}
