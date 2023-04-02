#pragma once
#include "Base.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

// This ignores all warnings raised inside external headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace TS_ENGINE {
	
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger>& GetCoreLogger()
		{ 
			return s_CoreLogger;
		}
	private:
		static Ref<spdlog::logger> s_CoreLogger;
	};
}

//template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
//inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
//{
//	return os << glm::to_string(vector);
//}
//
//template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
//inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
//{
//	return os << glm::to_string(matrix);
//}
//
//template<typename OStream, typename T, glm::qualifier Q>
//inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
//{
//	return os << glm::to_string(quaternion);
//}

// Core log macros
#define TS_CORE_TRACE(...)    ::TS_ENGINE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TS_CORE_INFO(...)     ::TS_ENGINE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TS_CORE_WARN(...)     ::TS_ENGINE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TS_CORE_ERROR(...)    ::TS_ENGINE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TS_CORE_CRITICAL(...) ::TS_ENGINE::Log::GetCoreLogger()->critical(__VA_ARGS__)


