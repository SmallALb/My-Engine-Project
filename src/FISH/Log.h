#pragma once
#define FMT_CONSTEXPR
#define SPDLOG_USE_STD_FORMAT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/fmt/ostr.h>

#include "Core.h"



namespace FISH {
    //日志类
    class API_ Log {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger();
        static std::shared_ptr<spdlog::logger>& GetClientLogger(); 
        static std::shared_ptr<spdlog::logger>& GetFilelogger();
    private:
        static std::shared_ptr<spdlog::logger>  s_CoreLogger;
        static std::shared_ptr<spdlog::logger>  s_ClientLogger;
        static std::shared_ptr<spdlog::logger>  file_logger;
        static string fileDir;
    };


}




#ifdef DEBUG
    //CORE
    #define FS_CORE_ERROR(...)  ::FISH::Log::GetCoreLogger()->error(__VA_ARGS__), nullptr
    #define FS_CORE_WARN(...)   ::FISH::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define FS_CORE_INFO(...)   ::FISH::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define FS_CORE_TRACE(...)  ::FISH::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define FS_CORE_FATAL(...)  ::FISH::Log::GetCoreLogger()->fatal(__VA_ARGS__)

    //USER
    #define FS_ERROR(...)  ::FISH::Log::GetClientLogger()->error(__VA_ARGS__), nullptr
    #define FS_WARN(...)   ::FISH::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define FS_INFO(...)   ::FISH::Log::GetClientLogger()->info(__VA_ARGS__)
    #define FS_TRACE(...)  ::FISH::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define FS_FATAL(...)  ::FISH::Log::GetClientLogger()->fatal(__VA_ARGS__)
    #define FS_ASSERT(x, ...) if (!x) {FS_ERROR("Get Fail!: {0}", __VA_ARGS__); ; }
    #define FS_CORE_ASSERT(x, ...) if (!x) {FS_CORE_ERROR("Get Fail!: {0}", __VA_ARGS__); ; }
    #define FS_CONDITION_INFO(x, ...) if ((x)) FISH::Log::GetCoreLogger()->info(__VA_ARGS__)
#else
    #define FS_CORE_ERROR(...) nullptr
    #define FS_CORE_WARN(...)
    #define FS_CORE_INFO(...)
    #define FS_CORE_TRACE(...)
    #define FS_CORE_FATAL(...)

    #define FS_ERROR(...) nullptr
    #define FS_WARN(...)
    #define FS_INFO(...)
    #define FS_TRACE(...)
    #define FS_FATAL(...)
    #define FS_ASSERT(x, ...)
    #define FS_CORE_ASSERT(x, ...)
    #define FS_CONDITION_INFO(x, ...)
#endif
// #else
//     #define FS_CORE_ERROR
//     #define FS_CORE_WARN
//     #define FS_CORE_INFO
//     #define FS_CORE_TRACE
//     #define FS_CORE_FATAL

//     #define FS_ERROR
//     #define FS_WARN
//     #define FS_INFO
//     #define FS_TRACE
//     #define FS_FATAL
// #endif
