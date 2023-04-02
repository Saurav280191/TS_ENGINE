#pragma once
#include "Core/tspch.h"
#include "Primitive/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

namespace TS_ENGINE {

	class Mesh;

	class GameObject
	{
	private:
		//GLuint mTextureID;
		Vector3 mPosition = Vector3(0);
		Vector3 mEulerAngle = Vector3(0);
		Vector3 mScale = Vector3(1);

		bool mHasTexture;
	protected:
		Vector3 mColor;
		Ref<TS_ENGINE::Texture2D> mTexture;

		Vector2 mTiling = Vector2(1);
		std::vector<Mesh*> mMeshes;
		const char* mName;
	public:
		GameObject();
		
		void SetName(const char* name);
		const char* GetName() const
		{
			return mName;
		}
		void SetPosition(Vector3 position);
		void SetPosition(float x, float y, float z);
		void SetEulerAngle(float x, float y, float z);
		void SetScale(float x);
		void SetScale(float x, float y, float z);

		Vector3 GetPosition();

		Vector3 GetEulerAngle();

		Vector3 GetScale();

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

		void AddMesh(Mesh* mesh);
		void AddMeshes(std::vector<Mesh*> _meshes);
		std::vector<Mesh*> GetMeshes();
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

