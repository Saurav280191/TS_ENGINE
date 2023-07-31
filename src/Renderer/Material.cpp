#include "tspch.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Material.h"

namespace TS_ENGINE {

	Material::Material(const std::string& name, Ref<Shader> shader) :
		mAmbientColor(1.0f),
		mDiffuseColor(1.0f),
		mSpecularColor(1.0f),
		mShininess(16.0f)
	{
		mName = name;
		mShader = shader;
	}

	void Material::SetAmbientColor(const Vector3& ambientColor)
	{
		mAmbientColor = ambientColor;
	}
	void Material::SetDiffuseColor(const Vector3& diffuseColor)
	{
		mDiffuseColor = diffuseColor;
	}
	void Material::SetSpecularColor(const Vector3& specularColor)
	{
		mSpecularColor = specularColor;
	}
	void Material::SetShininess(float shininess)
	{
		mShininess = shininess;
	}
	
	const Ref<Shader>& Material::GetShader()
	{
		return mShader;		
	}
}
