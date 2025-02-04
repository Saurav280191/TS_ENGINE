#pragma once
#include "SceneManager/Node.h"
#include "Primitive/Sphere.h"
#include "Primitive/Line.h"

namespace TS_ENGINE {

	struct VertexWeight 
	{
		unsigned int vertexId = 0;
		float weight = 0.0f;

		VertexWeight(aiVertexWeight _weight)
		{
			vertexId = _weight.mVertexId;
			weight = _weight.mWeight;
		}

		VertexWeight(int _vertexId, float _weight)
		{
			vertexId = _vertexId;
			weight = _weight;
		}
	};
	
	class Bone
	{
	public:		
		Bone();

		void SetParams(int _id, const Matrix4& _offsetMatrix);
		void SetNode(Ref<Node> _node);
		Ref<Node> GetNode();
		int GetId();

		// Creates joint and bone Guis for picking
		void CreateGui(const std::string& _name);

		void Update(Ref<Shader> _shader);
		void Render(Ref<Shader> _shader);

		void UpdateBoneGui(Ref<Node> _rootNode);

		bool PickNode(int _entityId);
	private:
		int mId;									// ID is index in finalBoneMatrices
		Matrix4 mOffsetMatrix;						// OffsetMatrix transforms vertex from model space to bone space
		Ref<Node> mNode;							// Node that will be effected by the bone			
		
		Matrix4 mBoneTransformMatrix;				// BoneTransformMatrix

		Ref<Node> mJointGuiNode;					// Gui to show joint
		std::vector<Ref<Node>> mBoneGuiNodes;		// Gui to show bone
		
	};
}