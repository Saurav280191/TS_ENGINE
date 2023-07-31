#include "tspch.h"
#include "GameObject.h"

namespace TS_ENGINE {

	GameObject::GameObject() :
		mHasTexture(false),
		mColor(Vector3(0.5f)),
		mDepthTestEnabled(true)
	{
		mEntityType = EntityType::GAMEOBJECT;
		mMeshes = {};
	}

	GameObject::GameObject(const std::string& name = "") :
		mHasTexture(false),
		mColor(Vector3(0.5f)),
		mDepthTestEnabled(true)
	{
		mName = name;
		mEntityType = EntityType::GAMEOBJECT;
		mMeshes = {};
	}

	GameObject::~GameObject()
	{
		mHasTexture = false;
		mColor = Vector3(0.0f);
		mMeshes.clear();
	}

	void GameObject::Initialize()
	{
		mEntityID = EntityManager::GetInstance()->Instantiate(mName, mEntityType);
	}
	void GameObject::SetName(const std::string& name)
	{
		mName = name;
		mNode->SetName(name);
	}
	void GameObject::Update(float deltaTime)
	{
		Draw();
	}

	void GameObject::SetColor(Vector3 color)
	{
		mColor.r = color.x;
		mColor.g = color.y;
		mColor.b = color.z;
		//mColor.a = a;
	}

	void GameObject::SetColor(float r, float g, float b)
	{
		mColor.r = r;
		mColor.g = g;
		mColor.b = b;
		//mColor.a = a;
	}

	void GameObject::SetColor32(float r, float g, float b)
	{
		mColor.r = r * 0.003921568627451f;
		mColor.g = g * 0.003921568627451f;
		mColor.b = b * 0.003921568627451f;
		//mColor.a = a;
	}

	const Vector3 GameObject::GetColor()
	{
		return mColor;
	}

	void GameObject::SetMaterial(Ref<Material> material)
	{
		mMaterial = material;
	}

	void GameObject::SetTexture(std::string path)
	{
		mHasTexture = true;
		mTexture = TS_ENGINE::Texture2D::Create("Assets\\Textures\\" + path);		
	}

	void GameObject::SetTexture(Ref<Texture2D> _texture)
	{
		mHasTexture = true;
		mTexture = _texture;
	}

	void GameObject::SetTexture(GLuint texID)
	{
		mHasTexture = true;
		mTexture = Texture2D::GetTextureFromID(texID);
	}

	GLuint GameObject::GetTextureID()
	{
		return mTexture->GetRendererID();
	}

	void GameObject::SetTextureTiling(int x, int y)
	{
		mTiling = Vector2(x, y);
	}

	//Shader over here is passed to copy the properties to fragment shader
	//void GameObject::Draw(Ref<Shader> shader)
	void GameObject::Draw()
	{
		RenderCommand::EnableDepthTest(mDepthTestEnabled);
		
		auto shader = mMaterial->GetShader();

		if (mTexture)
		{
			mTexture->Bind();			
			mMaterial->GetShader()->SetBool("u_HasTexture", true);
		}
		else
		{
			mMaterial->GetShader()->SetBool("u_HasTexture", false);
		}
		
		for(auto mesh : mMeshes)
			mesh->Draw();		

	}

	void GameObject::Destroy()
	{
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

	void GameObject::EnableDepthTest()
	{
		mDepthTestEnabled = true;
	}

	void GameObject::DisableDepthTest()
	{
		mDepthTestEnabled = false;
	}
}