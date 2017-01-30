#include "cursor.hpp"
#include "translationunit.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;

Cursor::Cursor(const ::CXCursor& cursor) :
    _cursor{cursor}
{}

Cursor Cursor::lexicalParent() const
{
    return ::clang_getCursorLexicalParent(cxCursor());
}

Cursor Cursor::semanticParent() const
{
    return ::clang_getCursorSemanticParent(cxCursor());
}

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

std::ostream& operator<<(std::ostream& os, const Cursor& cursor)
{
    return os << "{\"" << cursor.spelling()
        << "\" (' DisplayName: '" << cursor.displayName()
        << "', Kind: " << cursor.kind()
        << ", Hash: " << cursor.hash() << ")}";
}

bool operator==(const Cursor& lhs, const Cursor& rhs)
{
    return ::clang_equalCursors(lhs.cxCursor(), rhs.cxCursor());
}

bool operator!=(const Cursor& lhs, const Cursor& rhs)
{
    return !(lhs == rhs);
}

}

}

}

}

}

}
