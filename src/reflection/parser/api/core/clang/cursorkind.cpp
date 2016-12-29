#include "cursorkind.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;

::CXCursorKind convert(const CursorKind& kind)
{
    return static_cast<::CXCursorKind>(kind);
}

::CXCursorKind CursorKind::cxCursorKind() const
{
    return convert(*this);
}

String CursorKind::str() const
{
    return ::clang_getCursorKindSpelling(cxCursorKind());
}

bool CursorKind::isAttribute() const
{
    return ::clang_isAttribute(cxCursorKind());
}

bool CursorKind::isDeclaration() const
{
    return ::clang_isDeclaration(cxCursorKind());
}

bool CursorKind::isExpression() const
{
    return ::clang_isExpression(cxCursorKind());
}

bool CursorKind::isInvalid() const
{
    return ::clang_isInvalid(cxCursorKind());
}

bool CursorKind::isPreprocessing() const
{
    return ::clang_isPreprocessing(cxCursorKind());
}

bool CursorKind::isReference() const
{
    return ::clang_isReference(cxCursorKind());
}

bool CursorKind::isStatement() const
{
    return ::clang_isStatement(cxCursorKind());
}

bool CursorKind::isTranslationUnit() const
{
    return ::clang_isTranslationUnit(cxCursorKind());
}

bool CursorKind::isUnexposed() const
{
    return ::clang_isUnexposed(cxCursorKind());
}
