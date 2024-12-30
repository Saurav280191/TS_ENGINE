#include "Skybox.h"
#include "Sphere.h"
#include "Renderer/MaterialManager.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	Skybox::Skybox()
	{
		// Load skybox texture
		mSkyTexture = TS_ENGINE::Texture2D::Create(Application::s_AssetsDir.string() + "\\Textures\\Skybox\\industrial_sunset_puresky.jpg");
		
		// Create skybox mesh
		mMesh = CreateRef<Sphere>()->GetMesh();
		mMesh->SetName("Skybox");
		
		// Set material for mesh
		Ref<Material> unlitMaterial = TS_ENGINE::MaterialManager::GetInstance()->GetUnlitMaterial();
		mMesh->SetMaterial(unlitMaterial);

		// Set material texture
		mMesh->GetMaterial()->SetDiffuseMap(mSkyTexture);

		// Set Transform
		mTransform = CreateRef<Transform>();
		mTransform->SetLocalScale(1600.0f, 1600.0f, 1600.0f);
		mTransform->SetLocalEulerAngles(90.0f, 235.0f, 0.0f);

		mTransform->ComputeTransformationMatrix(nullptr);
	}

	Skybox::~Skybox()
	{
		TS_CORE_INFO("Deleted Skybox!");
	}

	void Skybox::Render()
	{	
		// Set shader properties for skybox
		Ref<Shader> shader = mMesh->GetMaterial()->GetShader();
		shader->SetMat4("u_Model", mTransform->GetGlobalTransformationMatrix());// Model Matrix
		shader->SetVec4("u_DiffuseColor", Vector4(0.8f, 0.8f, 0.8f, 1));		// Diffuse Color
		mSkyTexture->Bind();													// Bind Skybox Texture
		shader->SetBool("u_HasDiffuseTexture", true);							// Set HadDiffuseColor to true
		
		// Render Command To Draw Geometry
		RenderCommand::DrawIndexed(mMesh->GetVertexArray(), mMesh->GetNumIndices());

	}
}