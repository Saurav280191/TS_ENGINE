#include "tspch.h"
#include "MaterialManager.h"
#include "Core/Application.h"

namespace TS_ENGINE {

	Ref<MaterialManager> MaterialManager::mInstance = nullptr;

	Ref<MaterialManager> MaterialManager::GetInstance()
	{
		if (mInstance == nullptr)
			mInstance = CreateRef<MaterialManager>();

		return mInstance;
	}

	void MaterialManager::LoadAllShadersAndCreateMaterials()
	{
		// TODO: Modify the code to load all shaders automatically
		// TODO: Modify code to show material properties in material gui depending on shader uniform keywords

		// Shaders
		std::string shaderDir = Application::s_ResourcesDir.string() + "\\Shaders\\";
		mUnlitShader = Shader::Create("UnlitShader", shaderDir + "Unlit.vert", shaderDir + "Unlit.frag");
		mLitShader = Shader::Create("LitShader", shaderDir + "Lit.vert", shaderDir + "Lit.frag");
		//mHdrLitShader = Shader::Create("HDRLighting", shaderDir + "HDRLighting.vert", shaderDir + "HDRLighting.frag");
		//mBatchLitShader = Shader::Create("BatchLit", shaderDir + "BatchLit.vert", shaderDir + "BatchLit.frag");

		// Materials
		mUnlitMat = CreateRef<Material>("UnlitMaterial", mUnlitShader);// Create default material
		mLitMat = CreateRef<Material>("LitMaterial", mLitShader);
		//mHdrLitMat = CreateRef<Material>("UnlitMaterial", mUnlitShader);
		//mBatchLitMat = CreateRef<Material>("BatchMaterial", mBatchLitShader);
	}

	Ref<Material> MaterialManager::GetDefaultMaterial()
	{
		return mUnlitMat;
	}

	Ref<Material> MaterialManager::GetUnlitMaterial()
	{
		return mUnlitMat;
	}

	Ref<Material> MaterialManager::GetLitMaterial()
	{
		return mLitMat;
	}
}
