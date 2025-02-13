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
		Ref<Mesh> sphereMesh = CreateRef<Sphere>()->GetMesh();
		sphereMesh->SetName("Skybox");
		
		// Set material for mesh
		Ref<Material> unlitMaterial = CreateRef<Material>(*TS_ENGINE::MaterialManager::GetInstance()->GetMaterial("Unlit"));
		sphereMesh->SetMaterial(unlitMaterial);

		// Set material texture
		sphereMesh->GetMaterial()->SetAmbientColor(Vector4(0.8f, 0.8f, 0.8f, 1));
		sphereMesh->GetMaterial()->SetDiffuseMap(mSkyTexture);

		// Add sphere mesh to node
		AddMesh(sphereMesh);

		// Set Transform		
		mTransform->SetLocalScale(1600.0f, 1600.0f, 1600.0f);
		mTransform->SetLocalEulerAngles(Vector3(90.0f, 235.0f, 0.0f));

		mTransform->ComputeTransformationMatrix(nullptr);
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
		Ref<Shader> shader = GetMesh()->GetMaterial()->GetShader();

		// Send Skybox's nodeId to vertex shader 
#ifdef  TS_ENGINE_EDITOR
		shader->SetInt("u_NodeId", mId);										
#endif
		// Send Skybox's modelMatrix to vertex shader 
		shader->SetMat4("u_Model", mTransform->GetWorldTransformationMatrix());	// Model Matrix

		// Render Skybox's mesh 
#ifdef  TS_ENGINE_EDITOR
		GetMesh()->Render(mId, true);
#else
		GetMesh()->Render(true);
#endif

		// If wireframe mode is enabled, re-enable it for other meshes
		if (Application::GetInstance().IsWireframeModeEnabled())
		{
			RenderCommand::EnableWireframe(true);
		}
	}
}