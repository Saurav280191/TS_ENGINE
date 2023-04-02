#include "tspch.h"
#include "Core/Window.h"

#ifdef TS_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace TS_ENGINE
{
	Scope<Window> Window::Create(const WindowProperties& properties)
	{
#ifdef TS_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(properties);
#else
		TS_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}
