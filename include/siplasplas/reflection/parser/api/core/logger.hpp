#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_LOGGER_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_LOGGER_HPP

#include <siplasplas/logger/logger.hpp>
#include <siplasplas/reflection/parser/api/core/export.hpp>

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

/**
 * \brief Returns the logger of the siplasplas-reflection-parser-api-core module
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT spdlog::logger& log();

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_LOGGER_HPP
