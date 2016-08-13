#include "logger.hpp"

namespace cpp
{

namespace typeerasure
{

spdlog::logger& log()
{
    static std::shared_ptr<spdlog::logger> logger = []
    {
        return cpp::Logger::addLogger("typeerasure");
    }();

    return *logger;
}

}

}
