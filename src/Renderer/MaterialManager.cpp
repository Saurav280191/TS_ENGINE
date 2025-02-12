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
		TS_CORE_TRACE("Loading all shaders and materials");

		// TODO: Modify the code to load all shaders automatically
		// TODO: Modify code to show material properties in material gui depending on shader uniform keywords

		// Shaders
		std::string shaderDir = Application::s_ResourcesDir.string() + "\\Shaders\\";
		mUnlitShader = Shader::Create("UnlitShader", shaderDir + "Unlit.vert", shaderDir + "Unlit.frag");
		//mSkinnedMeshUnlitShader = Shader::Create("SkinnedMeshUnlitShader", shaderDir + "SkinnedMeshUnlit.vert", shaderDir + "Unlit.frag");
		mLitShader = Shader::Create("LitShader", shaderDir + "Lit.vert", shaderDir + "Lit.frag");
		//mHdrLitShader = Shader::Create("HDRLighting", shaderDir + "HDRLighting.vert", shaderDir + "HDRLighting.frag");
		//mBatchLitShader = Shader::Create("BatchLit", shaderDir + "BatchLit.vert", shaderDir + "BatchLit.frag");

		// Materials
		mUnlitMat = CreateRef<Material>("Unlit", mUnlitShader);										// Unlit/Default material
		mAllMaterials.insert({ "Unlit", mUnlitMat });
		
		mLitMat = CreateRef<Material>("Lit", mLitShader);											// Lit material
		mAllMaterials.insert({ "Lit", mLitMat });
		
		//mSkinnedMeshUnlitMat = CreateRef<Material>("SkinnedMeshUnlit", mSkinnedMeshUnlitShader);	// SkinnedMeshUnlitMat material
		//mAllMaterials.insert({ "SkinnedMeshUnlit", mSkinnedMeshUnlitMat });
		
		//mHdrLitMat = CreateRef<Material>("Hdr", mUnlitShader);									// HdrMaterial material
		//mAllMaterials.insert({ "Hdr", mHdrLitMat });
		
		//mBatchLitMat = CreateRef<Material>("Batch", mBatchLitShader);								// BatchMaterial material
		//mAllMaterials.insert({ "Batch", mBatchLitMat });

		TS_CORE_TRACE("Completed shader loading\n");
	}

	const Ref<Material>& MaterialManager::GetMaterial(const std::string& _materialName)
	{
		TS_CORE_ASSERT(mAllMaterials[_materialName], "These is no material named: " + _materialName);
		return mAllMaterials[_materialName];
	}
}