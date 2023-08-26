#include "tspch.h"
#include "Material.h"
#include "Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	Material::Material(const std::string& name, Ref<Shader> shader) :
		mName(name),
		mShader(shader),
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

	}

	void Material::Render(int entityID)
	{
		RenderCommand::EnableDepthTest(mDepthTestEnabled);
		RenderCommand::EnableAlphaBlending(mAlphaBlendingEnabled);

		if (mShader)
		{
			//Send EntityID to fragment shader
			mShader->SetInt("u_EntityID", entityID);

			mShader->SetVec4("u_AmbientColor", mAmbientColor);
			mShader->SetVec4("u_DiffuseColor", mDiffuseColor);
			mShader->SetVec4("u_SpecularColor", mSpecularColor);

			if (Ref<Texture2D> diffuseMap = mDiffuseMap)
			{
				diffuseMap->Bind();
				mShader->SetBool("u_HasDiffuseTexture", true);
				mShader->SetVec2("u_DiffuseMapOffset", mDiffuseMapOffset);
				mShader->SetVec2("u_DiffuseMapTiling", mDiffuseMapTiling);
			}
			else
			{
				mShader->SetBool("u_HasDiffuseTexture", false);
			}
		}
	}	
}
