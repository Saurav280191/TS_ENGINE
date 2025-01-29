#pragma once
#include "Core/tspch.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE {

	struct KeyPosition
	{
		double timestamp;
		Vector3 position;
	};
	
	struct KeyRotation
	{
		double timestamp;
		Quaternion rotation;
	};

	struct KeyScale
	{
		double timestamp;
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
		Animation(aiAnimation* _aiAnimation);
		~Animation();

		// Fetches node of loaded Model from Factory class by name
		void InitializeNodesForAnimation();
		void Update(double _deltaTime);
		void UpdateBoneTransforms();

		void Play();
		void Pause();
		void Stop();
		
		void ToggleIsPlaying();
		
		const std::string& GetName() { return mName; }
		double GetDuration() { return mDuration; }
		double GetCurrentTime() { return mCurrentTime; }
		bool IsPlaying() { return mIsPlaying; }
		const std::unordered_map<Ref<Node>, KeyTransforms>& GetNodeKeyTransformMap() { return mNodeAndKeyTransformsMap; }
	private:
		std::string mName;		// Name
		double mDuration;		// Duration
		double mCurrentTime;	// CurrentTime
		bool mIsPlaying;		// IsPlaying

		// Stores Bone Name and KeyTransforms
		std::unordered_map<std::string, KeyTransforms> mNodeNameAndKeyTransformsMap;
		// Stores Node and KeyTransforms
		std::unordered_map<Ref<Node>, KeyTransforms> mNodeAndKeyTransformsMap;
	};
}

