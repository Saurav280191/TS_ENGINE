#pragma once
#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace TS_ENGINE {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& properties);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

		//Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override
		{ 
			m_Data.EventCallback = callback;
		}
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void SetWireframeMode(bool enabled) override;

		virtual void* GetNativeWindow() const 
		{ 
			return m_Window;
		}
	private:
		virtual void Init(const WindowProperties& properties);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
