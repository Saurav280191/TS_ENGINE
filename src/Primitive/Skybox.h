#include "Renderer/Shader.h"
#include "Primitive/Mesh.h"
#include "Application.h"
#include "Transform.h"

namespace TS_ENGINE
{
	class Skybox
	{
	public:
		Skybox();
		~Skybox();

		void Render();
	private:
		Ref<Texture2D> mSkyTexture;
		Ref<Mesh> mMesh;
		Ref<Transform> mTransform;
	};
}