#include "tspch.h"
#include "ModelLoader.h"

namespace TS_ENGINE {
	
	ModelLoader::ModelLoader() :
		mLoadedModelMap(),
		mLastLoadedModel(NULL)
	{

	}
	
	ModelLoader::~ModelLoader()
	{
		for (auto& loadedModelPair : mLoadedModelMap)
			loadedModelPair.second->Destroy();

		mLastLoadedModel = NULL;
		mLoadedModelMap.clear();
	}

	void ModelLoader::LoadModel(const std::string& directory, const std::string& modelName)
	{
		std::string fullModelPath = directory + "\\" + modelName;

		if (!IsModelLoaded(fullModelPath))
		{
			mLastLoadedModel = CreateRef<Model>(fullModelPath);
			mLoadedModelMap[fullModelPath] = mLastLoadedModel;
		}
		else
		{
			mLastLoadedModel = CreateRef<Model>();//Create new instance and then copy data
			mLastLoadedModel->CopyFrom(mLoadedModelMap[fullModelPath]);
		}
	}

	bool ModelLoader::IsModelLoaded(const std::string& fullModelPath)
	{
		if (mLoadedModelMap.find(fullModelPath) != mLoadedModelMap.end())
			return true;
		
		return false;
	}
}
