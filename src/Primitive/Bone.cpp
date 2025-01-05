#include "Bone.h"
#include "Renderer/MaterialManager.h"
#include "Core/Factory.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {
	
	Bone::Bone() :
		mId(0),
		mVertexWeights({}),
		mOffsetMatrix(Matrix4(1)),
		mNode(nullptr),
		mJointGuiNode(nullptr),
		mBoneGuiNodes({})
	{

	}

	void Bone::SetParams(std::string& _name, int _id, std::vector<VertexWeight>& _vertexWeights, const Matrix4& _offsetMatrix)
	{
		//mGuiJointMesh->SetName(_name);
		mName = _name;

		mId = _id;
		mVertexWeights = _vertexWeights;
		mOffsetMatrix = _offsetMatrix;

		// Create sphere Gui to show joint
		mJointGuiNode = Factory::GetInstance()->InstantiateSphere(mName + "-SphereGui", nullptr);
		// Set mesh color to Orange
		mJointGuiNode->GetMesh()->GetMaterial()->SetAmbientColor(Vector4(1.0f, 0.647f, 0.0f, 1.0f));
	}

	void Bone::SetNode(Ref<Node> _node)
	{
		mNode = _node;
	}
	
	void Bone::InitializeBones()
	{
		// Create bone between node and it's children
		for (auto& child : mNode->GetChildren())
		{
			// Create bone Gui
			Ref<Node> boneGuiNode = Factory::GetInstance()->InstantiateCube(mName + "-BoneGui", nullptr);
			// Set mesh color to Orange
			boneGuiNode->GetMesh()->GetMaterial()->SetAmbientColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
			
			TS_CORE_INFO("Created bone between {0} and {1}", mNode->mName, child->mName);

			mBoneGuiNodes.push_back(boneGuiNode);
		}
	}

	void Bone::UpdateBoneGuiTransforms()
	{
		mJointGuiNode->mTransform->SetGlobalTransformationMatrix(mNode->GetTransform()->GetGlobalTransformationMatrix());

		for (int i = 0; i < mNode->GetChildCount(); i++)
		{
			mNode->GetTransform()->DecomposeGlobalTransform();
			mNode->GetChildAt(i)->GetTransform()->DecomposeGlobalTransform();

			Vector3 point1 = mNode->GetTransform()->GetPosition();
			Vector3 point2 = mNode->GetChildAt(i)->GetTransform()->GetPosition();
			glm::vec3 direction = glm::normalize(point1 - point2);
			glm::quat rotation = glm::rotation(glm::vec3(1, 0, 0), direction);
			float boneLength = glm::distance(point1, point2);

			mBoneGuiNodes[i]->GetTransform()->SetLocalPosition((point1 + point2) * 0.5f);
			mBoneGuiNodes[i]->GetTransform()->SetLocalRotation(rotation);
			mBoneGuiNodes[i]->GetTransform()->SetLocalScale(glm::vec3(boneLength, 0.01f, 0.01f));

			mBoneGuiNodes[i]->GetTransform()->ComputeTransformationMatrix(nullptr);
		}
	}

	void Bone::Render(Ref<Shader> _shader)
	{
		RenderCommand::EnableWireframe(false);

		// Render mJointGuiNode 
		_shader->SetMat4("u_Model", mJointGuiNode->mTransform->GetGlobalTransformationMatrix());
#ifdef TS_ENGINE_EDITOR
		mJointGuiNode->GetMesh()->Render(mEntity->GetEntityID());
#else
		mJointGuiNode->GetMesh()->Render();
#endif

		// Render all boneGuiNodes 
		for(auto& boneGuiNode : mBoneGuiNodes)
		{
			_shader->SetMat4("u_Model", boneGuiNode->mTransform->GetGlobalTransformationMatrix());
#ifdef TS_ENGINE_EDITOR		
			boneGuiNode->GetMesh()->Render(mEntity->GetEntityID());
#else		
			boneGuiNode->GetMesh()->Render();
#endif
		}

		if (Application::GetInstance().IsWireframeModeEnabled())
		{
			RenderCommand::EnableWireframe(true);
		}
	}
}