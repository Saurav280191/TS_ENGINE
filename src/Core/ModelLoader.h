#pragma once
#include "TS_ENGINE.h"
#include "Primitive/Model.h"

namespace TS_ENGINE {

	class ModelLoader
	{
	private:
		static ModelLoader* mInstance;
		std::unordered_map<std::string, Ref<Model>> mLoadedModelMap;
		Ref<Model> mLastLoadedModel;
	public:
		static ModelLoader* GetInstance();
		~ModelLoader();

		Ref<Model> LoadModel(const std::string& directory, const std::string& modelName);		
		const Ref<Model> GetLastLoadedModel() const
		{
			return mLastLoadedModel;
		};
		bool IsModelLoaded(const std::string& fullModelPath);
	};
}