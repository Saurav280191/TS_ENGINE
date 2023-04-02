#pragma once
#include "Renderer/Shader.h"

namespace TS_ENGINE {

	class OpenGLShader : public Shader
	{
	private:
		uint32_t mRendererID;
		std::string mName;
		void CheckCompileErrors(GLuint shader, std::string type);
	public:
		OpenGLShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		virtual ~OpenGLShader();


		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override;

		virtual void SetBool(const char* name, bool value) override;
		virtual void SetInt(const char* name, int value) override;
		virtual void SetIntArray(const char* name, unsigned int numValues, int values[]) override;
		virtual void SetFloatArray(const char* name, unsigned int numValues, float values[]) override;
		virtual void SetFloat(const char* name, float value) override;
		virtual void SetVec2(const char* name, Vector2 v) override;
		virtual void SetVec3(const char* name, Vector3 v) override;
		virtual void SetVec4(const char* name, Vector4 v) override;
		virtual void SetMat4(const char* name, Matrix4 matrix) override;
	};
}
