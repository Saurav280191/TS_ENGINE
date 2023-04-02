#pragma once
#include <memory>
#include <intrin.h>

//Base
#ifdef TS_DEBUG
	#if defined(TS_PLATFORM_WINDOWS)
		#define TS_DEBUGBREAK() __debugbreak()
	#endif
#define TS_ENABLE_ASSERTS
#endif

#define TS_EXPAND_MACRO(x) x
#define TS_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define TS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

//Unique pointer template
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args) ...);
}

//Shared pointer template
template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
} 

#include "Log.h"
#include "TSAssert.h"