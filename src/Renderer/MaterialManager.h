#pragma once
#include "Core/tspch.h"
#include "Core/Base.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Material.h"

namespace TS_ENGINE {

	class MaterialManager
	{
	public:
		static Ref<MaterialManager> GetInstance();
		void LoadAllShadersAndCreateMaterials();

		Ref<Material> GetUnlitMaterial();
		Ref<Material> GetSkinnedMeshUnlitMaterial();
		Ref<Material> GetLitMaterial();
		
		const char* mMaterialNameList[2] = {
			"Unlit",
			"Lit"
		};

		const std::vector<Ref<Material>> GetAllMaterials() { return mAllMaterials; }

	private:
		static Ref<MaterialManager> mInstance;		
		
		Ref<Shader> mUnlitShader;
		Ref<Shader> mSkinnedMeshUnlitShader;
		Ref<Shader> mLitShader;
		Ref<Shader> mHdrLitShader;
		Ref<Shader> mBatchLitShader;

		Ref<Material> mUnlitMat;
		Ref<Material> mSkinnedMeshUnlitMat;
		Ref<Material> mLitMat;
		Ref<Material> mHdrLitMat;
		Ref<Material> mBatchLitMat;

		std::vector<Ref<Material>> mAllMaterials;
	};
}

