#include "logger.hpp"

namespace cpp
{

namespace signals
{

spdlog::logger& log()
{
    static std::shared_ptr<spdlog::logger> logger = []
    {
        return cpp::Logger::addLogger("signals");
    }();

    return *logger;
}

}

}
