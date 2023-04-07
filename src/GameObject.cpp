#include "tspch.h"
#include "GameObject.h"

namespace TS_ENGINE {

	//TEST
	GameObject::GameObject() :
		mHasTexture(false),
		//mTextureID(0),
		mColor(Vector3(1, 1, 1))
	{
		//mMesh = CreateRef<Mesh>();
		mMeshes = {};
	}

	void GameObject::SetName(const char* name)
	{
		mName = name;
	}

	void GameObject::SetPosition(Vector3 position)
	{
		mPosition = position;
	}
	void GameObject::SetPosition(float x, float y, float z)
	{
		mPosition = glm::vec3(x, y, z);
	}
	void GameObject::SetEulerAngle(float x, float y, float z)
	{
		mEulerAngle = glm::vec3(x, y, z);
	}
	void GameObject::SetScale(float x)
	{
		mScale = glm::vec3(x);
	}
	void GameObject::SetScale(float x, float y, float z)
	{
		mScale = glm::vec3(x, y, z);
	}

	Vector3 GameObject::GetPosition()
	{
		return mPosition;
	}
	Vector3 GameObject::GetEulerAngle()
	{
		return mEulerAngle;
	}
	Vector3 GameObject::GetScale()
	{
		return mScale;
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

	void GameObject::Draw(Ref<Shader> shader)
	{
		if(mTexture)
			mTexture->Bind();

		shader->SetMat4("u_Model",
			glm::translate(glm::mat4(1), mPosition)
			* glm::rotate(glm::mat4(1), glm::radians(mEulerAngle.x), glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1), glm::radians(-mEulerAngle.y), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1), glm::radians(mEulerAngle.z), glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1), mScale));

		for(auto mesh : mMeshes)
			mesh->Draw();
	}

	void GameObject::Destroy()
	{
		for(auto mesh : mMeshes)
			mesh->Destroy();
	}

	void GameObject::AddMesh(Mesh* mesh)
	{
		mMeshes.push_back(mesh);
	}

	void GameObject::AddMeshes(std::vector<Mesh*> _meshes)
	{
		mMeshes = _meshes;
	}

	std::vector<Mesh*> GameObject::GetMeshes()
	{
		return mMeshes;
	}
}