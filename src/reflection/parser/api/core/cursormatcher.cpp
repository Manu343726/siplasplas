#include "cursormatcher.hpp"

using namespace cpp::reflection::parser::api::core;
using namespace cpp::reflection::parser::api::core::clang;

CursorMatcher::CursorMatcher(const CursorKind::Kind kind) :
    CursorMatcher{kind, ""}
{}

CursorMatcher::CursorMatcher(const CursorKind::Kind kind, const cpp::constexp::ConstStringView& spelling) :
    _kind{kind},
    _spelling{spelling.str()}
{}

CursorKind::Kind CursorMatcher::kind() const
{
    return _kind;
}

const std::string& CursorMatcher::spelling() const
{
    return _spelling;
}

bool CursorMatcher::matches(const Cursor& cursor) const
{
    return cursor.kind() == kind() &&
        (spelling().empty() || (spelling() == cursor.spelling()));
}
