#include "tspch.h"
#include "GameObject.h"

namespace TS_ENGINE {

	GameObject::GameObject() :
		mHasTexture(false),
		mColor(Vector3(1, 1, 1))
	{
		mMeshes = {};
		//mTransform = CreateRef<Transform>();
	}

	GameObject::~GameObject()
	{
		mHasTexture = false;
		mColor = Vector3(0.0f);
		mMeshes.clear();
	}

	void GameObject::SetName(std::string name)
	{
		mName = name;
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
		/*shader->SetMat4("u_Model",//Moved to Node class
			glm::translate(glm::mat4(1), mTransform->GetLocalPosition())
			* glm::rotate(glm::mat4(1), glm::radians(mTransform->GetLocalEulerAngles().x), glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1), glm::radians(-mTransform->GetLocalEulerAngles().y), glm::vec3(0, 1, 0))
			* glm::rotate(glm::mat4(1), glm::radians(mTransform->GetLocalEulerAngles().z), glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1), mTransform->GetLocalScale()));*/

		if(mTexture)
			mTexture->Bind();

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
}