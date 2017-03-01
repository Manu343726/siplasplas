#include "logger.hpp"
#include <siplasplas/utility/string.hpp>

using namespace cpp;

#ifndef SIPLASPLAS_LOG_LEVEL
#   define SIPLASPLAS_LOG_LEVEL trace
#endif
#ifndef SIPLASPLAS_LOG_ALWAYS_FLUSH
    static constexpr bool FORCE_FLUSH = true;
#else
    static constexpr bool FORCE_FLUSH = false;
#endif

static constexpr spdlog::level::level_enum LOG_LEVEL = spdlog::level::SIPLASPLAS_LOG_LEVEL;

std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> Logger::createSink(const std::string& name)
{
    constexpr std::size_t LOGGER_BUFFER_LENGTH = 1024*1024;
    constexpr std::size_t LOGGER_FILES_MAX = 10;
    return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(name,  "siplasplas.log", LOGGER_BUFFER_LENGTH, LOGGER_FILES_MAX);
}

std::shared_ptr<spdlog::sinks::dist_sink_mt>& Logger::commonSink()
{
    static auto commonSink = []
    {
        auto distSink = std::make_shared<spdlog::sinks::dist_sink_mt>();
        distSink->add_sink(createSink("global"));
        return distSink;
    }();

    return commonSink;
}

std::shared_ptr<spdlog::logger> Logger::addLogger(const std::string& name)
{
    auto distSink = std::make_shared<spdlog::sinks::dist_sink_mt>();
    distSink->add_sink(createSink(name));
    distSink->add_sink(commonSink());
    auto logger = std::make_shared<spdlog::logger>(name, distSink);
    logger->set_level(LOG_LEVEL);
    logger->flush_on(spdlog::level::debug);

    spdlog::register_logger(logger);

    logger->log(LOG_LEVEL, " ");
    logger->log(LOG_LEVEL, " ");
    logger->log(LOG_LEVEL, " ");
    logger->log(LOG_LEVEL, "{} logger registered", name);
    logger->log(LOG_LEVEL, " ");
    logger->log(LOG_LEVEL, " ");
    logger->log(LOG_LEVEL, " ");

    return logger;
}
