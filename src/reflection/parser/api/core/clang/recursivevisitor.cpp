#include "recursivevisitor.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;

Visitor::Result RecursiveVisitor::onCursor(Visitor::Tag, const Cursor& current, const Cursor& parent)
{
    if(!visit(current))
    {
        Result result = onCursor(Tag(), current, parent);

        if(result != Result::Break)
        {
            return Result::Continue;
        }
        else
        {
            return Result::Break;
        }
    }
    else
    {
        return Result::Break;
    }
}

Visitor::Result RecursiveVisitor::onCursor(Visitor::Tag, const Cursor& current, const Cursor& parent) const
{
    if(!visit(current))
    {
        Result result = onCursor(Tag(), current, parent);

        if(result != Result::Break)
        {
            return Result::Continue;
        }
        else
        {
            return Result::Break;
        }
    }
    else
    {
        return Result::Break;
    }
}

Visitor::Result RecursiveVisitor::onCursor(RecursiveVisitor::Tag, const Cursor& current, const Cursor& parent) const
{
    return Result::Break;
}

Visitor::Result RecursiveVisitor::onCursor(RecursiveVisitor::Tag, const Cursor& current, const Cursor& parent)
{
    return Result::Break;
}
