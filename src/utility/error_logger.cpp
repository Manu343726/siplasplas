#include "error_logger.hpp"

namespace cpp
{

namespace utility
{

spdlog::logger& errorLogger()
{
    static std::shared_ptr<spdlog::logger> logger = []
    {
        return Logger::addLogger("error");
    }();

    return *logger;
}

}

}
