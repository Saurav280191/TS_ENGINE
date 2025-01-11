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
		mTransform->SetLocalEulerAngles(Vector3(90.0f, 235.0f, 0.0f));

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
		// Make sure skybox is never rendered in wireframe
		RenderCommand::EnableWireframe(false);

		// Set shader properties for skybox
		Ref<Shader> shader = mMesh->GetMaterial()->GetShader();

		// Send Skybox's entityId to vertex shader 
#ifdef  TS_ENGINE_EDITOR
		shader->SetInt("u_EntityID", mEntity->GetEntityID());					// Entity ID
#endif
		// Send Skybox's modelMatrix to vertex shader 
		shader->SetMat4("u_Model", mTransform->GetWorldTransformationMatrix());	// Model Matrix

		// Render Skybox's mesh 
#ifdef  TS_ENGINE_EDITOR
		mMesh->Render(mEntity->GetEntityID(), true);
#else
		mMesh->Render(true);
#endif

		// If wireframe mode is enabled, re-enable it for other meshes
		if (Application::GetInstance().IsWireframeModeEnabled())
		{
			RenderCommand::EnableWireframe(true);
		}
	}

#ifdef TS_ENGINE_EDITOR
	Ref<Entity> Skybox::GetEntity()
	{
		return mEntity;
	}
#endif
}