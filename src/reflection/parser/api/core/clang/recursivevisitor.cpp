#include "recursivevisitor.hpp"
#include <siplasplas/utility/scopedincrement.hpp>

using namespace ::cpp::reflection::parser::api::core::clang;

Visitor::Result RecursiveVisitor::onCursor(Visitor::Tag, const Cursor& current, const Cursor& parent)
{
    cpp::ScopedSizeCounter scopedCounter{_depth};

    // Visit siblings first:
    Result result = onCursor(Tag(), current, parent);

    if(result == Result::Continue)
    {
        // Then visit children (breadth first traversal)
        if(visit(current))
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
    cpp::ScopedSizeCounter scopedCounter{_depth};

    // Visit siblings first:
    Result result = onCursor(Tag(), current, parent);

    if(result == Result::Continue)
    {
        // Then visit children (breadth first traversal)
        if(visit(current))
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

std::size_t RecursiveVisitor::depth() const
{
    return _depth;
}

void RecursiveVisitor::resetDepth()
{
    _depth = 0;
}
