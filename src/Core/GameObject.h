#pragma once
#include "Core/tspch.h"
#include "Core/Object.h"
#include "Primitive/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Material.h"
#include "Renderer/Texture.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	class Mesh;

	class GameObject : public Object
	{
	public:
		GameObject();
		GameObject(const std::string& name);
		~GameObject();

		// Inherited via Object
		void Initialize();
		void SetName(const std::string& name);
		void Update(float deltaTime);

		void SetColor(Vector3 color);

		void SetColor(float r, float g, float b);
		void SetColor32(float r, float g, float b);
		const Vector3 GetColor();
		void SetMaterial(Ref<Material> material);
		Ref<Material> GetMaterial()
		{
			return mMaterial;
		}
		void SetTexture(std::string path);
		void SetTexture(Ref<Texture2D> _texture);
		void SetTexture(GLuint texID);
		GLuint GetTextureID();

		void SetTextureTiling(int x, int y);		
		
		void Draw();
		void Destroy();

		void AddMesh(Ref<Mesh> mesh);
		void AddMeshes(std::vector<Ref<Mesh>> _meshes);
		std::vector<Ref<Mesh>> GetMeshes();
		const bool GetHasTexture()
		{
			return mHasTexture;
		}
		void SetHasTextureTrue()
		{
			mHasTexture = true;
		}
		
		void EnableDepthTest();
		void DisableDepthTest();
	private:
		bool mHasTexture;
		bool mDepthTestEnabled;
	protected:
		Vector3 mColor;
		Ref<Material> mMaterial;
		Ref<Texture2D> mTexture;

		Vector2 mTiling = Vector2(1);
		std::vector<Ref<Mesh>> mMeshes;

	};
}

