#pragma once
#include "Renderer/Camera.h"
#include <Renderer/VertexArray.h>
#include <Renderer/Shader.h>
#include "Renderer/RendererAPI.h"

namespace TS_ENGINE {

	class Renderer
	{
	private:
		struct SceneData
		{
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		};

		static Scope<SceneData> sSceneData;
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() {
			return RendererAPI::GetAPI();
		}
	};
}

