#pragma once
#include "Core/tspch.h"
#include "Core/Base.h"
#include <Renderer/Shader.h>
#include <Renderer/Texture.h>

namespace TS_ENGINE {

	class Material
	{
	public:
		Material(const std::string& name, Ref<Shader> shader);
		virtual ~Material() = default;

		// Ambient
		void SetAmbientColor(const Vector4& ambientColor);		
		Vector4 GetAmbientColor() const;
		// Diffuse
		void SetDiffuseColor(const Vector4& diffuseColor);
		void SetDiffuseMap(const Ref<Texture2D> diffuseMap);
		void SetDiffuseMapOffset(Vector2 offset);
		void SetDiffuseMapTiling(Vector2 tiling);
		Vector4 GetDiffuseColor() const;
		Ref<Texture2D> GetDiffuseMap() const;
		Vector2 GetDiffuseMapOffset() const;
		Vector2 GetDiffuseMapTiling() const;
		// Specular
		void SetSpecularColor(const Vector4& specularColor);
		void SetSpecularMap(const Ref<Texture2D> specularMap);
		void SetSpecularMapOffset(Vector2 offset);
		void SetSpecularMapTiling(Vector2 tiling);
		void SetShininess(float shininess);		
		Vector4 GetSpecularColor() const;
		Ref<Texture2D> GetSpecularMap() const;
		Vector2 GetSpecularMapOffset() const;
		Vector2 GetSpecularMapTiling() const;
		float GetShininess() const;
		// Bump
		void SetNormalMap(const Ref<Texture2D> normalMap);
		void SetNormalMapOffset(Vector2 offset);
		void SetNormalMapTiling(Vector2 tiling);
		void SetBumpValue(const float bumpValue);
		Ref<Texture2D> GetNormalMap() const;
		Vector2 GetNormalMapOffset() const;
		Vector2 GetNormalMapTiling() const;
		float GetBumpValue() const;		
		//Shader
		const Ref<Shader>& GetShader();
		
	private:
		std::string mName;
		Ref<Shader> mShader;
		
		// Ambient
		Vector4 mAmbientColor;
		// Diffuse
		Vector4 mDiffuseColor;
		Ref<Texture2D> mDiffuseMap;
		Vector2 mDiffuseMapOffset;
		Vector2 mDiffuseMapTiling;
		// Specular
		Vector4 mSpecularColor;
		Ref<Texture2D> mSpecularMap;
		Vector2 mSpecularMapOffset;
		Vector2 mSpecularMapTiling;
		float mShininess;
		// Normal
		Ref<Texture2D> mNormalMap;
		Vector2 mNormalMapOffset;
		Vector2 mNormalMapTiling;
		float mBumpValue;
	};
}

