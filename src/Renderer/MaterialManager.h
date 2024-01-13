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

		Ref<Material> GetDefaultMaterial();
		Ref<Material> GetUnlitMaterial();
		Ref<Material> GetLitMaterial();
	private:
		static Ref<MaterialManager> mInstance;		
		
		Ref<Shader> mUnlitShader;
		Ref<Shader> mLitShader;
		Ref<Shader> mHdrLitShader;
		Ref<Shader> mBatchLitShader;

		Ref<Material> mUnlitMat;
		Ref<Material> mLitMat;
		Ref<Material> mHdrLitMat;
		Ref<Material> mBatchLitMat;

	};
}

