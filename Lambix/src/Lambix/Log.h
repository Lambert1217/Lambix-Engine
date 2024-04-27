#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Lambix
{

	class Log
	{

	public:
		// 日志系统初始化
		static void init();

		// 内联函数将直接在函数被调用处替换成函数体，避免了函数调用时的栈帧创建和销毁
		inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;   // 引擎logger
		static std::shared_ptr<spdlog::logger> s_ClientLogger; // 用户logger
	};

}

// 引擎 logger 宏
#define LB_CORE_TRACE(...) ::Lambix::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LB_CORE_INFO(...) ::Lambix::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LB_CORE_WARN(...) ::Lambix::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LB_CORE_ERROR(...) ::Lambix::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LB_CORE_FATAL(...) ::Lambix::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// 引擎 logger 宏
#define LB_CLIENT_TRACE(...) ::Lambix::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LB_CLIENT_INFO(...) ::Lambix::Log::GetClientLogger()->info(__VA_ARGS__)
#define LB_CLIENT_WARN(...) ::Lambix::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LB_CLIENT_ERROR(...) ::Lambix::Log::GetClientLogger()->error(__VA_ARGS__)
#define LB_CLIENT_FATAL(...) ::Lambix::Log::GetClientLogger()->fatal(__VA_ARGS__)