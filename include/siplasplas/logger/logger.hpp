#ifndef SIPLASPLAS_LOGGER_LOGGER_HPP
#define SIPLASPLAS_LOGGER_LOGGER_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <vector>
#include <string>
#include <siplasplas/logger/export.hpp>

namespace cpp
{

class Logger
{
public:
    static std::shared_ptr<spdlog::logger> addLogger(const std::string& name);

private:
    static std::shared_ptr<spdlog::sinks::rotating_file_sink_mt>& commonSink();
    static std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> createSink(const std::string& name);
};

SIPLASPLAS_LOGGER_EXPORT spdlog::logger& log(const char* module);

}

#endif // SIPLASPLAS_LOGGER_LOGGER_HPP
