#include "logger.hpp"

namespace cpp
{

namespace utility
{

spdlog::logger& log()
{
    static std::shared_ptr<spdlog::logger> logger = []
    {
        return cpp::Logger::addLogger("utility");
    }();

    return *logger;
}

}

}
