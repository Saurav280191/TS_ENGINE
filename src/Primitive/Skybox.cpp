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
		mMesh->GetMaterial()->SetAmbientColor(Vector4(0.8f, 0.8f, 0.8f, 1));
		mMesh->GetMaterial()->SetDiffuseMap(mSkyTexture);

		// Set Transform
		mTransform = CreateRef<Transform>();
		mTransform->SetLocalScale(1600.0f, 1600.0f, 1600.0f);
		mTransform->SetLocalRotation(Vector3(90.0f, 235.0f, 0.0f));

		mTransform->ComputeTransformationMatrix(nullptr);

#ifdef TS_ENGINE_EDITOR
		// Register entity
		mEntity = EntityManager::GetInstance()->Register("Skybox", EntityType::SKYBOX);
#endif
	}

	Skybox::~Skybox()
	{
		TS_CORE_INFO("Deleted Skybox!");
	}

	void Skybox::Render()
	{	
		// Set shader properties for skybox
		Ref<Shader> shader = mMesh->GetMaterial()->GetShader();

#ifdef  TS_ENGINE_EDITOR
		shader->SetInt("u_EntityID", mEntity->GetEntityID());					// Entity ID
#endif

		shader->SetMat4("u_Model", mTransform->GetGlobalTransformationMatrix());// Model Matrix
		shader->SetVec4("u_DiffuseColor", Vector4(0.8f, 0.8f, 0.8f, 1));		// Diffuse Color
		mSkyTexture->Bind();													// Bind Skybox Texture
		shader->SetBool("u_HasDiffuseTexture", true);							// Set HadDiffuseColor to true
		
		// Render Command To Draw Geometry
		RenderCommand::DrawIndexed(mMesh->GetVertexArray(), mMesh->GetNumIndices());

	}

#ifdef TS_ENGINE_EDITOR
	Ref<Entity> Skybox::GetEntity()
	{
		return mEntity;
	}
#endif
}