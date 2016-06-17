#include "logger.hpp"

using namespace cpp;

std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> Logger::createSink(const std::string& name)
{
    constexpr std::size_t LOGGER_BUFFER_LENGTH = 1024*1024;
    constexpr std::size_t LOGGER_FILES_MAX = 10;
    return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(name, name + ".log", LOGGER_BUFFER_LENGTH, LOGGER_FILES_MAX);
}

std::shared_ptr<spdlog::sinks::rotating_file_sink_mt>& Logger::commonSink()
{
    static auto commonSink = createSink("siplasplas");
    return commonSink;
}

std::shared_ptr<spdlog::logger> Logger::addLogger(const std::string& name)
{
    auto distSink = std::make_shared<spdlog::sinks::dist_sink_mt>();
    distSink->add_sink(createSink(name));
    distSink->add_sink(commonSink());
    auto logger = std::make_shared<spdlog::logger>(name, distSink);

    spdlog::register_logger(logger);
    return logger;
}

namespace cpp
{

spdlog::logger& log(const char* module)
{
    
}

}



