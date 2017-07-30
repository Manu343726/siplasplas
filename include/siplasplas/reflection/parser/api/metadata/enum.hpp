#ifndef SIPLASPLAS_REFLECTION_PARSER_API_METADATA_ENUM
#define SIPLASPLAS_REFLECTION_PARSER_API_METADATA_ENUM

#include <siplasplas/reflection/parser/api/core/matchers.hpp>
#include <siplasplas/reflection/parser/api/metadata/export.hpp>

namespace cpp
{

namespace reflection
{

namespace parser
{

namespace api
{

namespace metadata
{

class SIPLASPLAS_REFLECTION_PARSER_API_METADATA_EXPORT Enum
{
public:
    static api::core::Matcher& matcher();
};

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_METADATA_ENUM
