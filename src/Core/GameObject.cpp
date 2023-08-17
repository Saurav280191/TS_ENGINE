#include "tspch.h"
#include "GameObject.h"

namespace TS_ENGINE {

	GameObject::GameObject()
	{
		mEntityType = EntityType::GAMEOBJECT;
		mNode->SetEntityType(EntityType::GAMEOBJECT);
		mMeshes = {};		
	}

	GameObject::GameObject(const std::string& name = "")
	{
		mName = name;
		mEntityType = EntityType::GAMEOBJECT;
		mNode->SetEntityType(EntityType::GAMEOBJECT);
		mMeshes = {};
	}

	GameObject::~GameObject()
	{				
		mMeshes.clear();
	}

	void GameObject::Initialize()
	{
		mEntityID = EntityManager::GetInstance()->Instantiate(mName, mEntityType);
	}
	
	void GameObject::Update(Ref<Shader> shader, float deltaTime)
	{
		//TS_CORE_TRACE("Rendering: {0}", mName);

		RenderCommand::EnableDepthTest(mDepthTestEnabled);
		RenderCommand::EnableAlphaBlending(mAlphaBlendingEnabled);

		Ref<Shader> materialShader = mMaterial->GetShader();

		materialShader->SetVec4("u_AmbientColor", mMaterial->GetAmbientColor());
		materialShader->SetVec4("u_DiffuseColor", mMaterial->GetDiffuseColor());
		materialShader->SetVec4("u_SpecularColor", mMaterial->GetSpecularColor());

		if (Ref<Texture2D> diffuseMap = mMaterial->GetDiffuseMap())
		{
			diffuseMap->Bind();
			materialShader->SetBool("u_HasDiffuseTexture", true);
			materialShader->SetVec2("u_DiffuseMapOffset", mMaterial->GetDiffuseMapOffset());
			materialShader->SetVec2("u_DiffuseMapTiling", mMaterial->GetDiffuseMapTiling());
		}
		else
		{
			materialShader->SetBool("u_HasDiffuseTexture", false);
		}


		for (auto& mesh : mMeshes)
			mesh->Draw();
	}

	void GameObject::DeleteMeshes()
	{
		for (auto& mesh : mMeshes)
		{
			mesh.reset();
		}

		mMeshes.clear();
	}
	
	void GameObject::Destroy()
	{
		//Destroy GameObject
		for(auto& mesh : mMeshes)
			mesh->Destroy();
	}

	void GameObject::AddMesh(Ref<Mesh> mesh)
	{
		mMeshes.push_back(mesh);
	}

	void GameObject::AddMeshes(std::vector<Ref<Mesh>> _meshes)
	{
		mMeshes = _meshes;
	}

	std::vector<Ref<Mesh>> GameObject::GetMeshes()
	{
		return mMeshes;
	}
}