#include "tspch.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Base.h"
#include "Renderer/Renderer.h"

namespace TS_ENGINE
{
	Application* Application::sInstance = NULL;
	std::chrono::time_point<std::chrono::steady_clock> finish;

	Application::Application()
	{
		TS_CORE_ASSERT(!sInstance, "Application already exists!");
		sInstance = this;

		mWindow = Window::Create();
		mWindow->SetEventCallback(TS_BIND_EVENT_FN(Application::OnEvent));
		mWindow->SetVSync(false);//V-Sync off

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(TS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(TS_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;

			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		while (mRunning)
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			mDeltaTime = (currentFrame - mLastFrame) * 1000.0f;
			mLastFrame = currentFrame;

			//TS_CORE_INFO("FPS: {0}, {1} ms/frame", 1000.0f / mDeltaTime, mDeltaTime);

			if (!mMinimized)
			{
				for (Layer* layer : mLayerStack)
					layer->OnUpdate(mDeltaTime);

				mImGuiLayer->Begin();
				{
					for (Layer* layer : mLayerStack)
						layer->OnImGUIRender();
				}
				mImGuiLayer->End();

				mWindow->OnUpdate();
			}
		}
	}

	void Application::Close()
	{
		mRunning = false;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mMinimized = true;
			return false;
		}

		mMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::AddDrawCalls(uint32_t drawcalls)
	{
		mDrawCalls += drawcalls;
	}

	void Application::AddVertices(uint32_t vertices)
	{
		mTotalVertices += vertices;
	}

	void Application::AddIndices(uint32_t indices)
	{
		mTotalIndices += indices;
	}

	void Application::ResetStats()
	{
		mDrawCalls = 0;
		mTotalVertices = 0;
		mTotalIndices = 0;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	const float Application::GetDeltaTime() const
	{
		return mDeltaTime;
	}

	const uint32_t Application::GetDrawCalls() const
	{
		return mDrawCalls;
	}

	const uint32_t Application::GetTotalVertices() const
	{
		return mTotalVertices;
	}

	const uint32_t Application::GetTotalIndices() const
	{
		return mTotalIndices;
	}

	void Application::ToggleWireframeMode()
	{
		mWireframeMode = !mWireframeMode;
		mWindow->SetWireframeMode(mWireframeMode);
	}
}