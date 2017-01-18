#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CURSORMATCHER_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CURSORMATCHER_HPP

#include "clang/cursor.hpp"
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

class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT CursorMatcher
{
public:
    CursorMatcher(const core::clang::CursorKind::Kind kind);
    CursorMatcher(const core::clang::CursorKind::Kind kind, const cpp::constexp::ConstStringView& spelling);

    core::clang::CursorKind::Kind kind() const;
    const std::string& spelling() const;

    bool matches(const core::clang::Cursor& cursor) const;

private:
    core::clang::CursorKind::Kind _kind;
    std::string _spelling;
};

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CURSORMATCHER_HPP
