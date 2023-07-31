#pragma once
#include "Core/tspch.h"
#include "Core/Base.h"
#include <Renderer/Shader.h>

namespace TS_ENGINE {

	class Material
	{
	public:
		Material(const std::string& name, Ref<Shader> shader);
		virtual ~Material() = default;

		void SetAmbientColor(const Vector3& ambientColor);
		void SetDiffuseColor(const Vector3& diffuseColor);
		void SetSpecularColor(const Vector3& specularColor);
		void SetShininess(float shininess);
		const Ref<Shader>& GetShader();
	private:
		std::string mName;
		Ref<Shader> mShader;
		Vector3 mAmbientColor;
		Vector3 mDiffuseColor;
		Vector3 mSpecularColor;
		float mShininess;		
	};
}

