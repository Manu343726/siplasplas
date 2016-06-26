#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_LOGGER_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_LOGGER_HPP

#include <siplasplas/logger/logger.hpp>
#include <siplasplas/reflection/dynamic/export.hpp>

namespace cpp
{

namespace reflection
{

namespace dynamic
{

SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT spdlog::logger& log();

}

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_LOGGER_HPP
