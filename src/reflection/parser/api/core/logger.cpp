#include "logger.hpp"

namespace cpp
{

namespace reflection
{

namespace parser
{

namespace api
{

namespace core
{

spdlog::logger& log()
{
    static std::shared_ptr<spdlog::logger> logger = []
    {
        return cpp::Logger::addLogger("reflection-parser-api-core");
    }();

    return *logger;
}

}

}

}

}

}
