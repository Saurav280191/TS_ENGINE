#include "tspch.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace TS_ENGINE {

	Ref<Shader> TS_ENGINE::Shader::Create(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::NONE:    TS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OPENGL:  return CreateRef<OpenGLShader>(shaderName, vertexShaderPath, fragmentShaderPath);
		}

		TS_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
