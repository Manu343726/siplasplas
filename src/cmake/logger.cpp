#include "logger.hpp"

namespace cpp
{

namespace cmake
{

spdlog::logger& log()
{
    static std::shared_ptr<spdlog::logger> logger = []
    {
        return cpp::Logger::addLogger("cmake");
    }();

    return *logger;
}

}

}
