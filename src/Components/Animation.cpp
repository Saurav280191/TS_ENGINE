#include "tspch.h"
#include "Animation.h"
#include "Core/Factory.h"

namespace TS_ENGINE {

    Animation::Animation(aiAnimation* _aiAnimation) :
        mName("AnimationNameNotSet"),
        mCurrentFrame(0),
        mTotalFrames(),
        mDuration(0.0f),
        mTicksPerSecond(0.0f),
        mCurrentTime(0.0f),
        mIsPlaying(false),
        mTotalTimeInSeconds(0.0f),
        mNodeNameAndKeyTransformsMap({}),
        mNodeAndKeyTransformsMap({})
	{
		//TS_CORE_TRACE("Animation " + std::to_string(i) + ": " + _aiAnimation->mName.C_Str());
		//TS_CORE_TRACE("Duration: " + std::to_string(_aiAnimation->mDuration));
		//TS_CORE_TRACE("Ticks per second: " + std::to_string(_aiAnimation->mTicksPerSecond));

        mName = _aiAnimation->mName.C_Str();
        mDuration = (float)_aiAnimation->mDuration;
        mTicksPerSecond = (float)_aiAnimation->mTicksPerSecond;
        float ticksPerSecond = (mTicksPerSecond != 0.0f) ? mTicksPerSecond : 24.0f;
        mTotalTimeInSeconds = mDuration / ticksPerSecond;
        mTotalFrames = (int)(mDuration * ticksPerSecond);

        aiNodeAnim** assimpAnimations = _aiAnimation->mChannels;

        for (unsigned int i = 0; i < _aiAnimation->mNumChannels; i++)
        {
            aiNodeAnim* assimpAnimation = assimpAnimations[i];
            std::string nodeName = assimpAnimation->mNodeName.C_Str();         

            KeyTransforms keyTransforms;

            for (unsigned int positionIndex = 0; positionIndex < assimpAnimation->mNumPositionKeys; ++positionIndex)
            {
                aiVector3D aiPosition = assimpAnimation->mPositionKeys[positionIndex].mValue;
                float timeStamp = (float)assimpAnimation->mPositionKeys[positionIndex].mTime;
                
                KeyPosition keyPosition;
                keyPosition.position = Vector3(aiPosition.x, aiPosition.y, aiPosition.z);
                keyPosition.timestamp = timeStamp;

                keyTransforms.mKeyPositions.push_back(keyPosition);
            }

            for (unsigned int rotationIndex = 0; rotationIndex < assimpAnimation->mNumRotationKeys; ++rotationIndex)
            {
                aiQuaternion aiOrientation = assimpAnimation->mRotationKeys[rotationIndex].mValue;
                float timeStamp = (float)assimpAnimation->mRotationKeys[rotationIndex].mTime;
                
                KeyRotation keyRotation;
                keyRotation.rotation = Quaternion(aiOrientation.w, aiOrientation.x, aiOrientation.y, aiOrientation.z);
                keyRotation.timestamp = timeStamp; 

                keyTransforms.mKeyRotations.push_back(keyRotation);
            }

            for (unsigned int scaleIndex = 0; scaleIndex < assimpAnimation->mNumScalingKeys; ++scaleIndex)
            {
                aiVector3D scale = assimpAnimation->mScalingKeys[scaleIndex].mValue;
                float timeStamp = (float)assimpAnimation->mScalingKeys[scaleIndex].mTime;
                
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
			if (const auto& node = Factory::GetInstance()->FindNodeByName(nodeName))
			{
				mNodeAndKeyTransformsMap.insert({ node, keyTransform });
			}
		}
    }

    void Animation::Update(float _deltaTime)
    {
        if (mIsPlaying)
        {
            mCurrentTime += _deltaTime;
        }

        // Calculate current time        
        (mCurrentTime > mTotalTimeInSeconds) ? mCurrentTime = 0.0f : void();

		// Convert to animation time (in ticks)
		float animationTime = mCurrentTime * mTicksPerSecond;
        animationTime = fmod(animationTime, mDuration);

		// Calculate the current frame index            
		mCurrentFrame = (mCurrentTime == mTotalTimeInSeconds) ? mCurrentFrame = static_cast<int>(mDuration) : static_cast<int>(animationTime); // Current frame index

        // Update Skeleton Hierarchy
		UpdateBoneTransforms(animationTime);
        
    }

    void Animation::UpdateBoneTransforms(float _animationTime)
    {
        for (auto& [node, keyTransforms] : mNodeAndKeyTransformsMap)
        {
            int positionKeyFrameIndex = FindPositionKeyFrameIndex(keyTransforms.mKeyPositions, _animationTime);
            int rotationKeyFrameIndex = FindRotationKeyFrameIndex(keyTransforms.mKeyRotations, _animationTime);
            int scaleKeyFrameIndex = FindScaleKeyFrameIndex(keyTransforms.mKeyScales, _animationTime);

            if (positionKeyFrameIndex >= 0 && positionKeyFrameIndex + 1 < keyTransforms.mKeyPositions.size())
            {
                auto& keyA = keyTransforms.mKeyPositions[positionKeyFrameIndex];
                auto& keyB = keyTransforms.mKeyPositions[positionKeyFrameIndex + 1];

                float factor = (_animationTime - keyA.timestamp) / (keyB.timestamp - keyA.timestamp);
                node->GetTransform()->SetLocalPosition(glm::mix(keyA.position, keyB.position, factor));
            }

            if (rotationKeyFrameIndex >= 0 && rotationKeyFrameIndex + 1 < keyTransforms.mKeyRotations.size())
            {
                auto& keyA = keyTransforms.mKeyRotations[rotationKeyFrameIndex];
                auto& keyB = keyTransforms.mKeyRotations[rotationKeyFrameIndex + 1];

                float factor = (_animationTime - keyA.timestamp) / (keyB.timestamp - keyA.timestamp);
                node->GetTransform()->SetLocalRotation(glm::slerp(keyA.rotation, keyB.rotation, factor));
            }

            if (scaleKeyFrameIndex >= 0 && scaleKeyFrameIndex + 1 < keyTransforms.mKeyScales.size())
            {
                auto& keyA = keyTransforms.mKeyScales[scaleKeyFrameIndex];
                auto& keyB = keyTransforms.mKeyScales[scaleKeyFrameIndex + 1];

                float factor = (_animationTime - keyA.timestamp) / (keyB.timestamp - keyA.timestamp);
                node->GetTransform()->SetLocalScale(glm::mix(keyA.scale, keyB.scale, factor));
            }
            
            // Computer node's transform
            node->ComputeTransformMatrices();
        }
    }

    int Animation::FindPositionKeyFrameIndex(const std::vector<TS_ENGINE::KeyPosition>& _keyPositions, double animationTime)
    {
        int keyCount = (int)_keyPositions.size(); // Assume using position keys

        if (keyCount == 0) return -1; // No keyframes

        for (int i = 0; i < keyCount - 1; i++)
        {
            if (animationTime < _keyPositions[i + 1].timestamp)
                return i; // Found the closest lower-bound keyframe
        }

        return keyCount - 1; // Return last frame if time exceeds
    }

    int Animation::FindRotationKeyFrameIndex(const std::vector<TS_ENGINE::KeyRotation>& _keyRotations, double animationTime)
    {
        int keyCount = (int)_keyRotations.size(); // Assume using position keys

        if (keyCount == 0) return -1; // No keyframes

        for (int i = 0; i < keyCount - 1; i++)
        {
            if (animationTime < _keyRotations[i + 1].timestamp)
                return i; // Found the closest lower-bound keyframe
        }

        return keyCount - 1; // Return last frame if time exceeds
    }

    int Animation::FindScaleKeyFrameIndex(const std::vector<TS_ENGINE::KeyScale>& _keyScales, double animationTime)
    {
        int keyCount = (int)_keyScales.size(); // Assume using position keys

        if (keyCount == 0) return -1; // No keyframes

        for (int i = 0; i < keyCount - 1; i++)
        {
            if (animationTime < _keyScales[i + 1].timestamp)
                return i; // Found the closest lower-bound keyframe
        }

        return keyCount - 1; // Return last frame if time exceeds
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
        mCurrentFrame = 0;
        mCurrentTime = 0.0f;
    }

    void Animation::ToggleIsPlaying()
    {
        mIsPlaying = !mIsPlaying;
    }
}