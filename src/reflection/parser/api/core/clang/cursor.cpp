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

std::string Cursor::fullQualifiedName() const
{
    if(semanticParent().isNull())
    {
        return spelling().str().str();
    }
    else
    {
        return semanticParent().fullQualifiedName() + "::" + spelling().str().str();
    }
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

std::size_t Cursor::depth() const
{
    return distanceToAncestor(translationUnit().cursor());
}

std::size_t Cursor::distanceToAncestor(const Cursor& ancestor) const
{
    if(ancestor == *this)
    {
        return 0;
    }
    else
    {
        return 1 + semanticParent().distanceToAncestor(ancestor);
    }
}

std::size_t Cursor::distanceToDescendant(const Cursor& descendant) const
{
    return descendant.distanceToAncestor(*this);
}

bool Cursor::isNull() const
{
    return ::clang_Cursor_isNull(cxCursor());
}

Cursor Cursor::null()
{
    return { ::clang_getNullCursor() };
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
        << "\" (Depth: " << cursor.depth()
        << ", DisplayName: '" << cursor.displayName()
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
