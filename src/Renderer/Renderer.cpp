#include "tspch.h"
#include "Renderer.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE {

	Scope<Renderer::SceneData> Renderer::sSceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		RenderCommand::Init();		
	}

	void Renderer::Shutdown()
	{
		
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera)
	{
		sSceneData->viewMatrix = camera.GetViewMatrix();
		sSceneData->projectionMatrix = camera.GetProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_View", sSceneData->viewMatrix);
		shader->SetMat4("u_Projection", sSceneData->projectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}
