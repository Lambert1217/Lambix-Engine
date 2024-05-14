#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Lambix
{

	// 静态变量在类外部定义
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::init()
	{
		// 设置日志输出格式
		spdlog::set_pattern("[%T] [%=3n] %^[%l]%$: %v");

		// 设置引擎logger的名称和等级
		s_CoreLogger = spdlog::stdout_color_mt("LBX");
		s_CoreLogger->set_level(spdlog::level::trace);

		// 设置用户logger的名称和等级
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

		LB_CORE_INFO("Log init!");
	}
}
