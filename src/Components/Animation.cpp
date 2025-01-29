#include "tspch.h"
#include "Animation.h"
#include "Core/Factory.h"

namespace TS_ENGINE {

    Animation::Animation(aiAnimation* _aiAnimation) :
        mName("AnimationNameNotSet"),
        mCurrentTime(0.0f),
        mDuration(0.0f),
        mIsPlaying(false),
        mNodeNameAndKeyTransformsMap({}),
        mNodeAndKeyTransformsMap({})
	{
		//TS_CORE_TRACE("Animation " + std::to_string(i) + ": " + _aiAnimation->mName.C_Str());
		//TS_CORE_TRACE("Duration: " + std::to_string(_aiAnimation->mDuration));
		//TS_CORE_TRACE("Ticks per second: " + std::to_string(_aiAnimation->mTicksPerSecond));

        mName = _aiAnimation->mName.C_Str();
        mDuration = (float)_aiAnimation->mDuration;
 
        aiNodeAnim** assimpAnimations = _aiAnimation->mChannels;

        for (unsigned int i = 0; i < _aiAnimation->mNumChannels; i++)
        {
            aiNodeAnim* assimpAnimation = assimpAnimations[i];
            std::string nodeName = assimpAnimation->mNodeName.C_Str();         

            KeyTransforms keyTransforms;

            for (unsigned int positionIndex = 0; positionIndex < assimpAnimation->mNumPositionKeys; ++positionIndex)
            {
                aiVector3D aiPosition = assimpAnimation->mPositionKeys[positionIndex].mValue;
                double timeStamp = (float)assimpAnimation->mPositionKeys[positionIndex].mTime;
                
                KeyPosition keyPosition;
                keyPosition.position = Vector3(aiPosition.x, aiPosition.y, aiPosition.z);
                keyPosition.timestamp = timeStamp;

                keyTransforms.mKeyPositions.push_back(keyPosition);
            }

            for (unsigned int rotationIndex = 0; rotationIndex < assimpAnimation->mNumRotationKeys; ++rotationIndex)
            {
                aiQuaternion aiOrientation = assimpAnimation->mRotationKeys[rotationIndex].mValue;
                double timeStamp = (float)assimpAnimation->mRotationKeys[rotationIndex].mTime;
                
                KeyRotation keyRotation;
                keyRotation.rotation = Quaternion(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z);
                keyRotation.timestamp = timeStamp; 

                keyTransforms.mKeyRotations.push_back(keyRotation);
            }

            for (unsigned int scaleIndex = 0; scaleIndex < assimpAnimation->mNumScalingKeys; ++scaleIndex)
            {
                aiVector3D scale = assimpAnimation->mScalingKeys[scaleIndex].mValue;
                double timeStamp = (float)assimpAnimation->mScalingKeys[scaleIndex].mTime;
                
                KeyScale keyScale;
                keyScale.scale = Vector3(scale.x, scale.y, scale.z);
                keyScale.timestamp = timeStamp;      

                keyTransforms.mKeyScales.push_back(keyScale);
            }           
            
            mNodeNameAndKeyTransformsMap.insert({ nodeName, keyTransforms });
        }
    }

	Animation::~Animation()
	{
        TS_CORE_INFO("Destroying Animation named: {0}", mName);
	}

    void Animation::InitializeNodesForAnimation()
    {
        mNodeAndKeyTransformsMap.clear();

		for (auto& [nodeName, keyTransform] : mNodeNameAndKeyTransformsMap)
		{
			// Search Nodes in Factory to fetch respective node with same nodeName 
			if (const auto& node = Factory::GetInstance()->GetNodeByName(nodeName))
			{
				mNodeAndKeyTransformsMap.insert({ node, keyTransform });
			}
		}
    }

    void Animation::Update(double _deltaTime)
    {
        if (mIsPlaying)
        {
            UpdateBoneTransforms();

            mCurrentTime += 0.1f * _deltaTime;

            if (mCurrentTime > mDuration)
            {
                mCurrentTime = 0.0;
            }
        }
    }

    void Animation::UpdateBoneTransforms()
    {
        for (auto& [node, keyTransforms] : mNodeAndKeyTransformsMap)
        {
            keyTransforms.mKeyPositions.size() > mCurrentTime ? node->GetTransform()->SetLocalPosition(keyTransforms.mKeyPositions[mCurrentTime].position) : (void)0;
            keyTransforms.mKeyRotations.size() > mCurrentTime ? node->GetTransform()->SetLocalRotation(keyTransforms.mKeyRotations[mCurrentTime].rotation) : (void)0;
            keyTransforms.mKeyScales.size() > mCurrentTime ? node->GetTransform()->SetLocalScale(keyTransforms.mKeyScales[mCurrentTime].scale) : (void)0;            
            node->ComputeTransformMatrices();
        }
    }

    void Animation::Play()
    {
        mIsPlaying = true;
    }

    void Animation::Pause()
    {
        mIsPlaying = false;
    }

    void Animation::Stop()
    {
        mIsPlaying = false;
        mCurrentTime = 0.0f;
    }

    void Animation::ToggleIsPlaying()
    {
        mIsPlaying = !mIsPlaying;
    }
}