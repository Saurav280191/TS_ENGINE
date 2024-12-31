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

#ifdef TS_ENGINE_EDITOR
		Ref<Entity> GetEntity();
#endif
	private:
#ifdef TS_ENGINE_EDITOR
		Ref<Entity> mEntity;
#endif
		Ref<Texture2D> mSkyTexture;
		Ref<Mesh> mMesh;
		Ref<Transform> mTransform;
	};
}