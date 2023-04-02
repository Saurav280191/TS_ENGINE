#pragma once
#include "Core/tspch.h"
#include "Core/Base.h"

namespace TS_ENGINE {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		virtual void SetBool(const char* name, bool value) = 0;
		virtual void SetInt(const char* name, int value) = 0;
		virtual void SetIntArray(const char* name, unsigned int numValues, int values[]) = 0;
		virtual void SetFloatArray(const char* name, unsigned int numValues, float values[]) = 0;
		virtual void SetFloat(const char* name, float value) = 0;
		virtual void SetVec2(const char* name, Vector2 v) = 0;
		virtual void SetVec3(const char* name, Vector3 v) = 0;
		virtual void SetVec4(const char* name, Vector4 v) = 0;
		virtual void SetMat4(const char* name, Matrix4 matrix) = 0;

		static Ref<Shader> Create(const std::string& shaderName, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	};
}

