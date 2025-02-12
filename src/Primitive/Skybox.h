#include "Renderer/Shader.h"
#include "Application.h"
#include "SceneManager/Node.h"

namespace TS_ENGINE
{
	class Skybox : public Node
	{
	public:
		Skybox();
		~Skybox();

		void Render();
	private:
		Ref<Texture2D> mSkyTexture;
	};
}