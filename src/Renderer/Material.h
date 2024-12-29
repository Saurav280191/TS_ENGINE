#pragma once
#include "Core/tspch.h"
#include "Core/Base.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace TS_ENGINE {

	class Material
	{
	public:
#ifdef TS_ENGINE_EDITOR
		struct MaterialGui
		{
			Vector4 mAmbientColor = Vector4(1, 1, 1, 1);
			Vector4 mDiffuseColor = Vector4(1, 1, 1, 1);
			Ref<Texture2D> mDiffuseMap = nullptr;
			float* mDiffuseMapOffset = nullptr;
			float* mDiffuseMapTiling = nullptr;
			Vector4 mSpecularColor = Vector4(1, 1, 1, 1);
			Ref<Texture2D> mSpecularMap = nullptr;
			float* mSpecularMapOffset = nullptr;
			float* mSpecularMapTiling = nullptr;
			float mShininess = 0.0f;
			Ref<Texture2D> mNormalMap = nullptr;
			float* mNormalMapOffset = nullptr;
			float* mNormalMapTiling = nullptr;
			float mBumpValue = 0.0f;
		};	
#endif
		enum TextureType
		{
			DIFFUSE,
			SPECULAR,
			NORMAL
		};

		Material();
		Material(const std::string& name, Ref<Shader> shader);
		Material(const Ref<Material>& material);
		virtual ~Material() = default;

		void CloneMaterialProperties(Ref<Material> material);

		const std::string& GetName() const { return mName;  }

		// Name
		void SetName(std::string name) { mName = name; }

		// Ambient
		void SetAmbientColor(const Vector4& ambientColor) { mAmbientColor = ambientColor; }
		Vector4 GetAmbientColor() const { return mAmbientColor; }

		// Diffuse
		void SetDiffuseColor(const Vector4& diffuseColor) { mDiffuseColor = diffuseColor; }
		void SetDiffuseMap(const Ref<Texture2D> diffuseMap) { mDiffuseMap = diffuseMap; }
		void SetDiffuseMapOffset(Vector2 offset) { mDiffuseMapOffset = offset; }
		void SetDiffuseMapTiling(Vector2 tiling) { mDiffuseMapTiling = tiling; }
		Vector4 GetDiffuseColor() const { return mDiffuseColor; }
		Ref<Texture2D> GetDiffuseMap() const { return mDiffuseMap; }
		Vector2 GetDiffuseMapOffset() const { return mDiffuseMapOffset; }
		Vector2 GetDiffuseMapTiling() const { return mDiffuseMapTiling; }

		// Specular
		void SetSpecularColor(const Vector4& specularColor) { mSpecularColor = specularColor; }
		void SetSpecularMap(const Ref<Texture2D> specularMap) { mSpecularMap = specularMap; }
		void SetSpecularMapOffset(Vector2 offset) { mSpecularMapOffset = offset; }
		void SetSpecularMapTiling(Vector2 tiling) { mSpecularMapTiling = tiling; }
		void SetShininess(float shininess) { mShininess = shininess; }
		Vector4 GetSpecularColor() const { return mSpecularColor; }
		Ref<Texture2D> GetSpecularMap() const { return mSpecularMap; }
		Vector2 GetSpecularMapOffset() const { return mSpecularMapOffset; }
		Vector2 GetSpecularMapTiling() const { return mSpecularMapTiling; }
		float GetShininess() const { return mShininess; }

		// Bump
		void SetNormalMap(const Ref<Texture2D> normalMap) { mNormalMap = normalMap; }
		void SetNormalMapOffset(Vector2 offset) { mNormalMapOffset = offset; }
		void SetNormalMapTiling(Vector2 tiling) { mNormalMapTiling = tiling; }
		void SetBumpValue(const float bumpValue) { mBumpValue = bumpValue; }
		Ref<Texture2D> GetNormalMap() const { return mNormalMap; }
		Vector2 GetNormalMapOffset() const { return mNormalMapOffset; }
		Vector2 GetNormalMapTiling() const { return mNormalMapTiling; }
		float GetBumpValue() const { return mBumpValue; }

		// Shader
		const Ref<Shader> GetShader();

		// Other material properties
		void EnableDepthTest() { mDepthTestEnabled = true; }
		void DisableDepthTest() { mDepthTestEnabled = false; }
		void EnableAlphaBlending() { mAlphaBlendingEnabled = true; }
		void DisableAlphaBlending() { mAlphaBlendingEnabled = false; }

		// Material Render
#ifdef  TS_ENGINE_EDITOR
		void Render(int entityID);
#else
		void Render();
#endif //  TS_ENGINE_EDITOR

#ifdef TS_ENGINE_EDITOR
		void SetMaterialGui(MaterialGui materialGui);		
		void ShowGUI(int meshIndex, bool treeOpen = true);
		void DropContentBrowserTexture(TextureType textureType, Material::MaterialGui& materialGui, int meshIndex);
#endif
	private:
#ifdef TS_ENGINE_EDITOR
		MaterialGui mMaterialGui;
#endif
		std::string mName;
		Ref<Shader> mShader;

		// Ambient
		Vector4 mAmbientColor = Vector4(0.8f, 0.8f, 0.8f, 1);
		// Diffuse
		Vector4 mDiffuseColor = Vector4(0.8f, 0.8f, 0.8f, 1);
		Ref<Texture2D> mDiffuseMap = nullptr;
		Vector2 mDiffuseMapOffset = Vector2(0, 0);
		Vector2 mDiffuseMapTiling = Vector2(1, 1);
		// Specular
		Vector4 mSpecularColor = Vector4(0.8f, 0.8f, 0.8f, 1);
		Ref<Texture2D> mSpecularMap = nullptr;
		Vector2 mSpecularMapOffset = Vector2(0, 0);
		Vector2 mSpecularMapTiling = Vector2(1, 1);
		float mShininess = 0.0f;
		// Normal
		Ref<Texture2D> mNormalMap = nullptr;
		Vector2 mNormalMapOffset = Vector2(0, 0);
		Vector2 mNormalMapTiling = Vector2(1, 1);
		float mBumpValue = 0.0f;

		//Other material properties
		bool mDepthTestEnabled;
		bool mAlphaBlendingEnabled;
	};
}

