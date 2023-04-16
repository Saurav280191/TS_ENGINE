#pragma once
#include "Core/tspch.h"
#include "Primitive/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
//#include "Core/Transform.h"

namespace TS_ENGINE {

	class Mesh;

	class GameObject
	{
	private:
		//Ref<Transform> mTransform;
		bool mHasTexture;
	protected:
		Vector3 mColor;
		Ref<TS_ENGINE::Texture2D> mTexture;

		Vector2 mTiling = Vector2(1);
		std::vector<Ref<Mesh>> mMeshes;
		std::string mName;
	public:
		GameObject();
		~GameObject();
		
		void SetName(std::string name);
		const std::string& GetName() const
		{
			return mName;
		}

		void SetColor(Vector3 color);

		void SetColor(float r, float g, float b);
		void SetColor32(float r, float g, float b);
		const Vector3 GetColor();
		void SetTexture(std::string path);
		void SetTexture(Ref<Texture2D> _texture);
		void SetTexture(GLuint texID);
		GLuint GetTextureID();

		void SetTextureTiling(int x, int y);

		void Draw(Ref<Shader> shader);

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
	};
}

