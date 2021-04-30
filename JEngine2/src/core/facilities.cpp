#include "pch.h"
#include "facilities.h"
#include "spdlog/sinks/stdout_color_sinks.h"
namespace JEngine2 {

	JEngine2::Logger::LoggerPtr Logger::Create()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		return spdlog::stdout_color_mt("JEngine");
	}

	JEngine2::Logger::LoggerPtr& Logger::GetLoggerPtr()
	{
		static auto mLogger = Create();
		return mLogger;
	}

}