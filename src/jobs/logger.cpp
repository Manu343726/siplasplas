#include "logger.hpp"

namespace cpp
{

namespace jobs
{

spdlog::logger& log()
{
    static std::shared_ptr<spdlog::logger> logger = []
    {
        return cpp::Logger::addLogger("jobs");
    }();

    return *logger;
}

}

}
