#include "tspch.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/Material.h"

namespace TS_ENGINE {

	Material::Material(const std::string& name, Ref<Shader> shader) :
		mAmbientColor(1.0f),
		mDiffuseColor(1.0f),
		mDiffuseMapOffset(0),
		mDiffuseMapTiling(1),
		mSpecularColor(1.0f),
		mSpecularMapOffset(0),
		mSpecularMapTiling(1),
		mShininess(16.0f),
		mNormalMapOffset(0),
		mNormalMapTiling(1),
		mBumpValue(1.0f)
	{
		mName = name;
		mShader = shader;
	}

	// Ambient
	void Material::SetAmbientColor(const Vector4& ambientColor)
	{
		mAmbientColor = ambientColor;
	}
	Vector4 Material::GetAmbientColor() const
	{
		return mAmbientColor;
	}
	
	// Diffuse
	void Material::SetDiffuseColor(const Vector4& diffuseColor)
	{
		mDiffuseColor = diffuseColor;
	}
	void Material::SetDiffuseMap(const Ref<Texture2D> diffuseMap)
	{
		mDiffuseMap = diffuseMap;
	}
	void Material::SetDiffuseMapOffset(Vector2 offset)
	{
		mDiffuseMapOffset = offset;
	}
	void Material::SetDiffuseMapTiling(Vector2 tiling)
	{
		mDiffuseMapTiling = tiling;
	}
	Ref<Texture2D> Material::GetDiffuseMap() const
	{
		return mDiffuseMap;
	}
	Vector2 Material::GetDiffuseMapOffset() const
	{
		return mDiffuseMapOffset;
	}
	Vector2 Material::GetDiffuseMapTiling() const
	{
		return mDiffuseMapTiling;
	}
	Vector4 Material::GetDiffuseColor() const
	{
		return mDiffuseColor;
	}
	
	// Specular
	void Material::SetSpecularColor(const Vector4& specularColor)
	{
		mSpecularColor = specularColor;
	}
	void Material::SetSpecularMap(const Ref<Texture2D> specularMap)
	{
		mSpecularMap = specularMap;
	}
	void Material::SetSpecularMapOffset(Vector2 offset)
	{
		mSpecularMapOffset = offset;
	}
	void Material::SetSpecularMapTiling(Vector2 tiling)
	{
		mSpecularMapTiling = tiling;
	}
	void Material::SetShininess(float shininess)
	{
		mShininess = shininess;
	}
	Vector4 Material::GetSpecularColor() const
	{
		return mSpecularColor;
	}
	Ref<Texture2D> Material::GetSpecularMap() const
	{
		return mSpecularMap;
	}
	Vector2 Material::GetSpecularMapOffset() const
	{
		return mSpecularMapOffset;
	}
	Vector2 Material::GetSpecularMapTiling() const
	{
		return mSpecularMapTiling;
	}
	float Material::GetShininess() const
	{
		return mShininess;
	}	
	
	// Normal
	void Material::SetNormalMap(const Ref<Texture2D> normalMap)
	{
		mNormalMap = normalMap;
	}
	void Material::SetNormalMapOffset(Vector2 offset)
	{
		mNormalMapOffset = offset;
	}
	void Material::SetNormalMapTiling(Vector2 tiling)
	{
		mNormalMapTiling = tiling;
	}
	void Material::SetBumpValue(const float bumpValue)
	{
		mBumpValue = bumpValue;
	}
	Ref<Texture2D> Material::GetNormalMap() const
	{
		return mNormalMap;
	}
	Vector2 Material::GetNormalMapOffset() const
	{
		return mNormalMapOffset;
	}
	Vector2 Material::GetNormalMapTiling() const
	{
		return mNormalMapTiling;
	}
	float Material::GetBumpValue() const
	{
		return mBumpValue;
	}
	
	//Shader
	const Ref<Shader>& Material::GetShader()
	{
		return mShader;		
	}
}
