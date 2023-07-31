#include "tspch.h"
#include "SceneCamera.h"
#include "Input.h"
#include <GLM/gtx/quaternion.hpp>

namespace TS_ENGINE
{
	SceneCamera::SceneCamera(const std::string& name) : Camera(name)
	{
		mName = name;
		mCameraType = CameraType::SCENECAMERA;

		mSkyboxShader = TS_ENGINE::Shader::Create("HDRLighting", "HDRLighting.vert", "HDRLighting.frag");
		mSkyboxMat = CreateRef<TS_ENGINE::Material>("SkyboxMaterial", mSkyboxShader);//Create default material

		mCurrentShader = mSkyboxShader;
	}
	void SceneCamera::Initialize()
	{
		mEntityID = EntityManager::GetInstance()->Instantiate(mName, mEntityType);
	}
	void SceneCamera::SetName(const std::string& name)
	{
		mName = name;
		mNode->SetName(name);
	}
	void SceneCamera::Update(float deltaTime)
	{
		//mNode->UpdateTransformMatrices();

		mViewMatrix = mNode->GetTransform()->GetTransformationMatrix();
		mViewMatrix = glm::inverse(mViewMatrix);
		
		/*TS_CORE_INFO("SceneCamera pos: {0}, {1}, {2}", 
			mNode->GetTransform()->GetLocalPosition().x, 
			mNode->GetTransform()->GetLocalPosition().y, 
			mNode->GetTransform()->GetLocalPosition().z);*/
		
		//float* viewMatPtr = glm::value_ptr(mViewMatrix);
		
		/*TS_CORE_INFO("mViewMatrix: \n{0}, {1}, {2}, {3}\n {4}, {5}, {6}, {7}\n {8}, {9}, {10}, {11}\n {12}, {13}, {14}, {15}", 
			viewMatPtr[0], viewMatPtr[1], viewMatPtr[2], viewMatPtr[3],
			viewMatPtr[4], viewMatPtr[5], viewMatPtr[6], viewMatPtr[7],
			viewMatPtr[8], viewMatPtr[9], viewMatPtr[10], viewMatPtr[11],
			viewMatPtr[12], viewMatPtr[13], viewMatPtr[14], viewMatPtr[15]);*/

		//Controls(deltaTime);

		mCurrentShader->SetVec3("u_ViewPos", mNode->GetTransform()->GetLocalPosition());
		mCurrentShader->SetMat4("u_View", mViewMatrix);
		mCurrentShader->SetMat4("u_Projection", mProjectionMatrix);
	}
}
