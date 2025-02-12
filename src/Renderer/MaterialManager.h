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
		
		const char* mMaterialNameList[2] = {
			"Unlit",
			"Lit"
		};

		const Ref<Material>& GetMaterial(const std::string& _materialName);

	private:
		static Ref<MaterialManager> mInstance;		
		
		Ref<Shader> mUnlitShader;
		Ref<Shader> mLitShader;
		//Ref<Shader> mSkinnedMeshUnlitShader;
		//Ref<Shader> mHdrLitShader;
		//Ref<Shader> mBatchLitShader;

		Ref<Material> mUnlitMat;
		Ref<Material> mLitMat;
		//Ref<Material> mSkinnedMeshUnlitMat;
		//Ref<Material> mHdrLitMat;
		//Ref<Material> mBatchLitMat;

		std::unordered_map<std::string, Ref<Material>> mAllMaterials;
	};
}

