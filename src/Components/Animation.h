#pragma once
#include "Core/tspch.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE {

	struct KeyPosition
	{
		float timestamp;
		Vector3 position;
	};
	
	struct KeyRotation
	{
		float timestamp;
		Quaternion rotation;
	};

	struct KeyScale
	{
		float timestamp;
		Vector3 scale;
	};

	struct KeyTransforms
	{
		std::vector<KeyPosition> mKeyPositions;
		std::vector<KeyRotation> mKeyRotations;
		std::vector<KeyScale> mKeyScales;
	};

	class Node;
	class Animation
	{
	public:
		Animation();
		Animation(aiAnimation* _aiAnimation);
		~Animation();

		// Fetches node of loaded Model from Factory class by name
		void InitializeNodesForAnimation(const Ref<Node>& _node);
		void Update(float _deltaTime);
		void UpdateBoneTransforms(float _animationTime);

		int FindPositionKeyFrameIndex(const std::vector<TS_ENGINE::KeyPosition>& _keyPositions, double animationTime);
		int FindRotationKeyFrameIndex(const std::vector<TS_ENGINE::KeyRotation>& _keyRotations, double animationTime);
		int FindScaleKeyFrameIndex(const std::vector<TS_ENGINE::KeyScale>& _keyScales, double animationTime);

		void Play();
		void Pause();
		void Stop();
		
		void ToggleIsPlaying();
		
		const std::string& GetName() { return mName; }
		float GetDuration() { return mDuration; }
		bool IsPlaying() { return mIsPlaying; }

		void CopyFrom(const Ref<Animation>& _other);

		std::unordered_map<Ref<Node>, KeyTransforms>& GetNodeKeyTransformMap() { return mNodeAndKeyTransformsMap; }
		
		int mCurrentFrame;			// CurrentFrame
		int mTotalFrames;			// TotalFrames
		float mTicksPerSecond;		// TicksPerSecond
		float mCurrentTime;			// CurrentTime
		float mTotalTimeInSeconds;	// Total 
		
		bool mInitializedNodesForAnimation;	// InitializedNodesForAnimation
		// Stores Node and KeyTransforms
		std::unordered_map<Ref<Node>, KeyTransforms> mNodeAndKeyTransformsMap;
	private:
		std::string mName;			// Name
		float mDuration;			// Duration
		bool mIsPlaying;			// IsPlaying

		// Stores Bone Name and KeyTransforms
		std::unordered_map<std::string, KeyTransforms> mNodeNameAndKeyTransformsMap;
	};
}

