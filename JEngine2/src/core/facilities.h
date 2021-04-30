#pragma once
#include "spdlog/spdlog.h"

namespace JEngine2 {
	//static Logger singleton
	class Logger
	{
		using LoggerPtr = std::shared_ptr<spdlog::logger>;
	public:
		static LoggerPtr Create();
		static LoggerPtr& GetLoggerPtr();
	};
}

#define JE_LOG_ERROR(...)    ::JEngine2::Logger::GetLoggerPtr()->error(__VA_ARGS__)
#define JE_LOG_WARN(...)     ::JEngine2::Logger::GetLoggerPtr()->warn(__VA_ARGS__)
#define JE_LOG_INFO(...)     ::JEngine2::Logger::GetLoggerPtr()->info(__VA_ARGS__)
#define JE_LOG_TRACE(...)    ::JEngine2::Logger::GetLoggerPtr()->trace(__VA_ARGS__)
#define JE_LOG_FATAL(...)    ::JEngine2::Logger::GetLoggerPtr()->fatal(__VA_ARGS__)