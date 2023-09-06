#include "tspch.h"
#include "Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace TS_ENGINE {

	OpenGLShader::OpenGLShader(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		mName = shaderName;

		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		std::string vsFullPath = vertexShaderPath;
		std::string fsFullPath = fragmentShaderPath;

		try
		{
			vShaderFile.open(vsFullPath);
			fShaderFile.open(fsFullPath);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			TS_CORE_ERROR("ERROR:SHADER::UNABLE_TO_LOAD_FILE: {0}", e.what());
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShaderID, 1, &vShaderCode, NULL);
		glCompileShader(vShaderID);
		CheckCompileErrors(vShaderID, "VERTEX");

		GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShaderID, 1, &fShaderCode, NULL);
		glCompileShader(fShaderID);
		CheckCompileErrors(fShaderID, "FRAGMENT");

		mRendererID = glCreateProgram();
		glAttachShader(mRendererID, vShaderID);
		glAttachShader(mRendererID, fShaderID);
		glLinkProgram(mRendererID);
		CheckCompileErrors(mRendererID, "PROGRAM");

		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);
	}

	OpenGLShader::~OpenGLShader()
	{

	}

	void OpenGLShader::CheckCompileErrors(GLuint shader, std::string type)
	{
		int success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				TS_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}", type.c_str(), infoLog);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				TS_CORE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type:{0}\n{1", type.c_str(), infoLog);
			}
		}
	}


	void OpenGLShader::Bind() const
	{
		TS_CORE_INFO("Switched shader to {0}", mName);
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	const std::string& OpenGLShader::GetName() const
	{
		return mName;
	}

	void OpenGLShader::SetBool(const char* name, bool value)
	{
		glUniform1i(glGetUniformLocation(mRendererID, name), (int)value);
	}

	void OpenGLShader::SetInt(const char* name, int value)
	{
		glUniform1i(glGetUniformLocation(mRendererID, name), value);
	}

	void OpenGLShader::SetIntArray(const char* name, unsigned int numValues, int values[])
	{
		glUniform1iv(glGetUniformLocation(mRendererID, name), numValues, values);
	}

	void OpenGLShader::SetFloatArray(const char* name, unsigned int numValues, float values[])
	{
		glUniform1fv(glGetUniformLocation(mRendererID, name), numValues, values);
	}

	void OpenGLShader::SetFloat(const char* name, float value)
	{
		glUniform1f(glGetUniformLocation(mRendererID, name), value);
	}

	void OpenGLShader::SetVec2(const char* name, Vector2 v)
	{
		const GLfloat f[] = { v.x, v.y };
		glUniform2fv(glGetUniformLocation(mRendererID, name), 1, f);
	}

	void OpenGLShader::SetVec3(const char* name, Vector3 v)
	{
		const GLfloat f[] = { v.x, v.y, v.z };
		glUniform3fv(glGetUniformLocation(mRendererID, name), 1, f);
	}

	void OpenGLShader::SetVec4(const char* name, Vector4 v)
	{
		const GLfloat f[] = { v.x, v.y, v.z, v.w };
		glUniform4fv(glGetUniformLocation(mRendererID, name), 1, f);
	}

	void OpenGLShader::SetMat4(const char* name, Matrix4 matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(mRendererID, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
}