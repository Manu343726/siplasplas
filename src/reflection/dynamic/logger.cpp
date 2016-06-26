#include "logger.hpp"

using namespace cpp;

namespace cpp
{

namespace reflection
{

namespace dynamic
{

spdlog::logger& log()
{
    static std::shared_ptr<spdlog::logger> logger = cpp::Logger::addLogger("reflection-dynamic");
    return *logger;
}

}

}

}
