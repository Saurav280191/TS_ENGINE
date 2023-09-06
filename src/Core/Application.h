#pragma once
#include "Core/Window.h"
#include <Events/ApplicationEvent.h>
#include <Core/LayerStack.h>
#include "ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace TS_ENGINE
{
	class Application {

	public:
		Application();
		virtual ~Application();

		static Application& Get()
		{
			return *sInstance;
		}

		Window& GetWindow()
		{
			return *mWindow;
		}

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void OnEvent(Event& e);
		void Run();
		void Close();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		float mLastFrame;
		
		void AddDrawCalls(uint32_t drawcalls);
		void AddVertices(uint32_t vertices);
		void AddIndices(uint32_t indices);

		const float GetDeltaTime() const;
		const uint32_t GetDrawCalls() const;
		const uint32_t GetTotalVertices() const;
		const uint32_t GetTotalIndices() const;
		
		void ResetStats();

		void ToggleWireframeMode();

		void ResetDrawCall()
		{
			mDrawCalls = 0;
		}

		static void SetExecutableDirectory(std::filesystem::path exeDir);
	public:
		static std::filesystem::path s_ExecutableDir;
		static std::filesystem::path s_AssetsDir;
		static std::filesystem::path s_ResourcesDir;
		static std::filesystem::path s_SaveSceneDir;
		static std::filesystem::path s_ThumbnailsDir;
	private:
		static Application* sInstance;		

		friend int ::main(int argc, char** argv);

		Scope<Window> mWindow;
		ImGuiLayer* mImGuiLayer;

		float mDeltaTime;
		uint32_t mDrawCalls;
		uint32_t mTotalVertices;
		uint32_t mTotalIndices;

		bool mRunning = true;
		bool mMinimized = false;
		LayerStack mLayerStack;
		bool mWireframeMode = false;
	};

	//To be defined in client
	Application* CreateApplication(std::filesystem::path exeDir);
}
