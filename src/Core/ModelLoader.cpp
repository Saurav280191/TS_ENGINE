#include "tspch.h"
#include "ModelLoader.h"

namespace TS_ENGINE {
	
	ModelLoader* ModelLoader::mInstance = nullptr;

	ModelLoader* ModelLoader::GetInstance()
	{
		if (mInstance == nullptr)
			mInstance = new ModelLoader();

		return mInstance;
	}
	
	ModelLoader::~ModelLoader()
	{
		for (auto& loadedModelPair : mLoadedModelMap)
			loadedModelPair.second->Destroy();

		mLastLoadedModel = nullptr;
		mLoadedModelMap.clear();
	}

	Ref<Model> ModelLoader::LoadModel(const std::string& directory, const std::string& modelName)
	{
		std::string fullModelPath = directory + "\\" + modelName;

		if (!IsModelLoaded(fullModelPath))
		{
			mLastLoadedModel = CreateRef<Model>(fullModelPath);
			mLoadedModelMap[fullModelPath] = mLastLoadedModel;
			return mLastLoadedModel;
		}
		else
		{
			mLastLoadedModel = CreateRef<Model>();//Create new instance and then copy data
			mLastLoadedModel->CopyFrom(mLoadedModelMap[fullModelPath]);
			return mLastLoadedModel;
		}
	}

	bool ModelLoader::IsModelLoaded(const std::string& fullModelPath)
	{
		if (mLoadedModelMap.find(fullModelPath) != mLoadedModelMap.end())
			return true;
		
		return false;
	}
}
