#include "cursor.hpp"
#include "translationunit.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;

Cursor::Cursor(const ::CXCursor& cursor) :
    _cursor{cursor}
{}

const ::CXCursor& Cursor::cxCursor() const
{
    return _cursor;
}

String Cursor::spelling() const
{
    return ::clang_getCursorSpelling(cxCursor());
}

String Cursor::displayName() const
{
    return ::clang_getCursorDisplayName(cxCursor());
}

CursorKind Cursor::kind() const
{
    return ::clang_getCursorKind(cxCursor());
}

TranslationUnit Cursor::translationUnit() const
{
    return TranslationUnit::proxy(::clang_Cursor_getTranslationUnit(cxCursor()));
}

std::size_t Cursor::hash() const
{
    return ::clang_hashCursor(cxCursor());
}

bool Cursor::isNull() const
{
    return ::clang_Cursor_isNull(cxCursor());
}
