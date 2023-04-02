#pragma once
#include "Base.h"
#include <filesystem>

#ifdef TS_ENABLE_ASSERTS

	#define TS_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { TS##type##ERROR(msg, __VA_ARGS__); TS_DEBUGBREAK(); }}
	#define TS_INTERNAL_ASSERT_WITH_MSG(type, check, ...) TS_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define TS_INTERNAL_ASSERT_NO_MSG(type, check) TS_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", TS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)
	
	#define TS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define TS_INTERNAL_ASSERT_GET_MACRO(...) TS_EXPAND_MACRO( TS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, TS_INTERNAL_ASSERT_WITH_MSG, TS_INTERNAL_ASSERT_NO_MSG) )
	
	#define TS_ASSERT(...) TS_EXPAND_MACRO( TS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define TS_CORE_ASSERT(...) TS_EXPAND_MACRO(TS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define TS_ASSERT(...)
	#define TS_CORE_ASSERT(...)
#endif
