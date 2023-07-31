#include "tspch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace TS_ENGINE {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		TS_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		TS_CORE_ASSERT(status, "Failed to initialize glad!");

		TS_CORE_INFO("OpenGL Info:");

		std::stringstream gl_vendor; gl_vendor << glGetString(GL_VENDOR);
		std::stringstream gl_renderer; gl_renderer << glGetString(GL_RENDERER);
		std::stringstream gl_version; gl_version << glGetString(GL_VERSION);

		TS_CORE_INFO(" Vendor: {0}", gl_vendor.str());
		TS_CORE_INFO(" Renderer: {0}", gl_renderer.str());
		TS_CORE_INFO(" Version: {0}", gl_version.str());

		TS_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "TS_ENGINE required at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}