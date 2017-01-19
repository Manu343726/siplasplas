#include "visitorinterface.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;

Visitor::Result VisitorInterface::onCursor(const Cursor& current, const Cursor& parent) const
{
    return Visitor::Result::Break;
}

Visitor::Result VisitorInterface::onCursor(const Cursor& current, const Cursor& parent)
{
    return Visitor::Result::Break;
}
