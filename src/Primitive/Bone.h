#pragma once
#include "SceneManager/Node.h"
#include "Primitive/Sphere.h"
#include "Primitive/Line.h"

namespace TS_ENGINE {
	
	class Bone
	{
	public:		
		Bone();

		void Update(Ref<Shader> _shader);

		bool PickNode(int _nodeId);
		
		void SetParams(int _id, const Matrix4& _offsetMatrix);
		void SetNode(Ref<Node> _node);
		Ref<Node> GetNode();
		int GetId();

		// Creates joint and bone Guis for picking
		void CreateGui();
		
		Matrix4 GetOffsetMatrix() { return mOffsetMatrix; }
	private:
		void UpdateBoneGui();
		void RenderBoneGui(Ref<Shader> _shader);

		int mId;									// ID is index in finalBoneMatrices
		Matrix4 mOffsetMatrix;						// OffsetMatrix transforms vertex from model space to bone space
		Ref<Node> mNode;							// Node that will be effected by the bone			
		
		Matrix4 mBoneTransformMatrix;				// BoneTransformMatrix = node->worldTransformationMatrix * mOffsetMatrix 

		Ref<Node> mJointGuiNode;					// Gui to show joint
		std::vector<Ref<Node>> mBoneGuiNodes;		// Gui to show bone
		
	};
}