#include "fspcs.h"
#include "Log.h"

namespace FISH{
    string Log::fileDir = "Logs";
    std::shared_ptr<spdlog::logger> Log::s_CoreLogger = nullptr;
    std::shared_ptr<spdlog::logger> Log::s_ClientLogger = nullptr;
    std::shared_ptr<spdlog::logger> Log::file_logger = nullptr;

    void Log::Init() {

        if (!std::filesystem::exists(fileDir)) std::filesystem::create_directories(fileDir);
        
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(fileDir + "/log_", 0, 0);
        // Core logger
        std::vector<spdlog::sink_ptr> sinks1 { console_sink, file_sink };
        auto coreLogger = std::make_shared<spdlog::logger>("FISH", sinks1.begin(), sinks1.end());
        coreLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
        coreLogger->set_level(spdlog::level::trace);

        // Client logger
        auto clientLogger = std::make_shared<spdlog::logger>("APP", sinks1.begin(), sinks1.end());
        clientLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
        clientLogger->set_level(spdlog::level::trace);

        s_CoreLogger = coreLogger;
        s_ClientLogger = clientLogger;
        file_logger = coreLogger; // 或 clientLogger，任选一个


        GetFilelogger()->warn("\n\n==========================[BEGIN LOG]========================\n");

    }

    std::shared_ptr<spdlog::logger> &Log::GetCoreLogger(){
        // if (s_CoreLogger == nullptr)
        //     s_CoreLogger = spdlog::stdout_color_mt("FISH");
        // s_CoreLogger->set_level(spdlog::level::trace);
        return s_CoreLogger;
    }
    std::shared_ptr<spdlog::logger> &Log::GetClientLogger(){
        // if (s_ClientLogger == nullptr)
        //     s_ClientLogger = spdlog::stdout_color_mt("APP");
        // s_ClientLogger->set_level(spdlog::level::trace);
        return s_ClientLogger;
    }

    std::shared_ptr<spdlog::logger> &Log::GetFilelogger() {
        // if (file_logger == nullptr) {
        //     file_logger = spdlog::daily_logger_mt("File", fileDir + "/log_", 0, 0);
        // }
        return file_logger;
    }
}