#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_KINDVISITOR_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_KINDVISITOR_HPP

#include "visitor.hpp"

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

template<core::clang::CursorKind::Kind Kind>
class KindVisitor : public core::clang::Visitor
{
public:
    virtual Visitor::Result onKind(const Cursor& current, const Cursor& parent) const
    {
        return Visitor::Result::Break;
    }

    virtual Visitor::Result onKind(const Cursor& current, const Cursor& parent)
    {
        return Visitor::Result::Break;
    }

    virtual Visitor::Result onIgnoredKind(const Cursor& current, const Cursor& parent) const
    {
        return Visitor::Result::Continue;
    }

    virtual Visitor::Result onIgnoredKind(const Cursor& current, const Cursor& parent)
    {
        return Visitor::Result::Continue;
    }

private:
    Visitor::Result onCursor(const Cursor& current, const Cursor& parent) const override
    {
        if(current.kind() == Kind)
        {
            return onKind(current, parent);
        }
        else
        {
            return onIgnoredKind(current, parent);
        }
    }

    Visitor::Result onCursor(const Cursor& current, const Cursor& parent) override
    {
        if(current.kind() == Kind)
        {
            return onKind(current, parent);
        }
        else
        {
            return onIgnoredKind(current, parent);
        }
    }
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_KINDVISITOR_HPP
