#pragma once
#include "Core/Base.h"
#include "Events/Event.h"
#include <sstream>

namespace TS_ENGINE {

	struct WindowProperties
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(const std::string& title = "TS_ENGINE", uint32_t width = 1920, uint32_t height = 1080) :
			Title(title),
			Width(width),
			Height(height)
		{

		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetWireframeMode(bool enabled) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProperties& properties = WindowProperties());
	};
}
