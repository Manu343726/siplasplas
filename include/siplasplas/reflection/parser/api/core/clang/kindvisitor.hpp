#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_KINDVISITOR_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_KINDVISITOR_HPP

#include "filteredvisitor.hpp"

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

namespace clang
{

namespace visitor_tags
{

template<core::clang::CursorKind::Kind CursorKind>
struct Kind {};

}

template<core::clang::CursorKind::Kind Kind>
struct KindVisitorPredicate
{
    bool operator()(const Cursor& current, const Cursor& parent = Cursor()) const
    {
        return current.kind() == Kind;
    }
};

template<core::clang::CursorKind::Kind Kind, typename Visitor = core::clang::Visitor>
using KindVisitor = core::clang::FilteredVisitor<
    core::clang::KindVisitorPredicate<Kind>,
    Visitor,
    core::clang::visitor_tags::Kind<Kind>
>;

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_KINDVISITOR_HPP
