#include "visitor.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;

bool Visitor::visit(const Cursor& cursor) const
{
    return ::clang_visitChildren(
        cursor.cxCursor(),
        +[](::CXCursor current, ::CXCursor parent, CXClientData userData)
        {
            const auto* self = const_cast<const Visitor*>(reinterpret_cast<Visitor*>(userData));
            return static_cast<::CXChildVisitResult>(self->onCursor(
                Tag(), Cursor{current}, Cursor{parent}
            ));
        },
        reinterpret_cast<void*>(const_cast<Visitor*>(this))
    );
}

bool Visitor::visit(const Cursor& cursor)
{
    return ::clang_visitChildren(
        cursor.cxCursor(),
        +[](::CXCursor current, ::CXCursor parent, CXClientData userData)
        {
            auto* self = reinterpret_cast<Visitor*>(userData);
            return static_cast<::CXChildVisitResult>(self->onCursor(
                Tag(), Cursor{current}, Cursor{parent}
            ));
        },
        reinterpret_cast<void*>(this)
    );
}

Visitor::Result Visitor::onCursor(Visitor::Tag, const Cursor& current, const Cursor& parent) const
{
    return Result::Break;
}

Visitor::Result Visitor::onCursor(Visitor::Tag, const Cursor& current, const Cursor& parent)
{
    return Result::Break;
}
