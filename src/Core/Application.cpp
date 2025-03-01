#include "tspch.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Core/Base.h"
//#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

namespace TS_ENGINE
{
	Application* Application::mInstance = NULL;

	std::filesystem::path Application::s_ExecutableDir;
	std::filesystem::path Application::s_AssetsDir;
	std::filesystem::path Application::s_ResourcesDir;
	std::filesystem::path Application::s_SaveSceneDir;
	std::filesystem::path Application::s_ThumbnailsDir;

	std::chrono::time_point<std::chrono::steady_clock> finish;

	Application::Application()
	{
		TS_CORE_ASSERT(!mInstance, "Application already exists!");
		mInstance = this;

		// Create window
		mWindow = Window::Create();
		mWindow->SetEventCallback(TS_BIND_EVENT_FN(Application::OnEvent));
		mWindow->SetVSync(false);// V-Sync off

		//Renderer::Init();

		// Create ImGui layer
		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);
	}

	Application::~Application()
	{
		//Renderer::Shutdown();
		TS_CORE_INFO("Deleting application");
	}

	Application& Application::GetInstance()
	{
		return *mInstance;
	}

	Window& Application::GetWindow()
	{		
		return *mWindow;		
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

	void Application::SetExecutableDirectory(std::filesystem::path exeDir)
	{
		s_ExecutableDir = exeDir;
		s_AssetsDir = std::string(exeDir.string() + "\\Assets");
		s_ResourcesDir = std::string(exeDir.string() + "\\Resources");
		s_SaveSceneDir = std::string(exeDir.string() + "\\Assets\\SavedScenes");
		s_ThumbnailsDir = std::string(exeDir.string() + "\\Resources\\SavedSceneThumbnails");
	}

	void Application::Run()
	{
		while (mRunning)
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			mDeltaTime = currentFrame - mLastFrame; // deltaTime in seconds
			mLastFrame = currentFrame;

			//TS_CORE_INFO("FPS: {0}, {1} ms/frame", 1000.0f / mDeltaTime, mDeltaTime);

			if (!mMinimized)
			{
				// Render scene
				for (Layer* layer : mLayerStack)
				{
					//glEnable(GL_POLYGON_SMOOTH);
					layer->OnUpdate(mDeltaTime);
				}

				// Render GUI
				mImGuiLayer->Begin();				
				//glDisable(GL_POLYGON_SMOOTH);
				for (Layer* layer : mLayerStack)
				{
					layer->OnImGuiRender();
				}
				mImGuiLayer->End();

				mWindow->OnUpdate();
			}
		}
	}

	void Application::Close()
	{
		TS_CORE_INFO("Closing application");
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
		//Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

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
		Close();
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
		RenderCommand::EnableWireframe(mWireframeMode);
	}
	
	void Application::ToggleTextures()
	{
		mTextureModeEnabled = !mTextureModeEnabled;
	}
	
	void Application::ToggleBoneView()
	{
		mBoneView = !mBoneView;
	}
	
	void Application::ToggleBoneInfluence()
	{
		mBoneInfluence = !mBoneInfluence;
	}

	void Application::ResetDrawCall()
	{		
		mDrawCalls = 0;
	}

	bool Application::IsWireframeModeEnabled()
	{
		return mWireframeMode;
	}

	bool Application::IsTextureModeEnabled()
	{
		return mTextureModeEnabled;
	}
}