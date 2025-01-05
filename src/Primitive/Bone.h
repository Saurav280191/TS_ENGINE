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

		void SetParams(std::string& _name, int _id, std::vector<VertexWeight>& _vertexWeights, const Matrix4& _offsetMatrix);
		void SetNode(Ref<Node> _node);

		void InitializeBones();

		void UpdateBoneGuiTransforms();

		void Render(Ref<Shader> _shader);
	private:
		std::string mName;							// Name
		int mId;									// Id
		std::vector<VertexWeight> mVertexWeights;	// VertexWeights
		Matrix4 mOffsetMatrix;						// OffsetMatrix
		
		// This node will be affect by the bone
		Ref<Node> mNode;							// Node					
		
		Ref<Node> mJointGuiNode;					// Gui to show joint
		std::vector<Ref<Node>> mBoneGuiNodes;		// Gui to show bone
	};
}