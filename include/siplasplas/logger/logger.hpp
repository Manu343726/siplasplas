#ifndef SIPLASPLAS_LOGGER_LOGGER_HPP
#define SIPLASPLAS_LOGGER_LOGGER_HPP

#include <spdlog/fmt/ostr.h>
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
    static std::shared_ptr<spdlog::sinks::dist_sink_mt>& commonSink();
    static std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> createSink(const std::string& name);
};

}

#endif // SIPLASPLAS_LOGGER_LOGGER_HPP
