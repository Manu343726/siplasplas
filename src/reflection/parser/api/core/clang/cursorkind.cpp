#include "cursorkind.hpp"
#include "detail/cursorkindenumserialization.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

CursorKind::CursorKind(const ::CXCursorKind kind) :
    _kind{static_cast<Kind>(kind)}
{}

CursorKind::Kind CursorKind::kind() const
{
    return _kind;
}

ConstStringView CursorKind::str() const
{
    return ::simpleKindString(cxCursorKind());
}

::CXCursorKind CursorKind::cxCursorKind() const
{
    return static_cast<::CXCursorKind>(kind());
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

std::ostream& operator<<(std::ostream& os, const CursorKind::Kind kind)
{
    return os << ::simpleKindString(static_cast<::CXCursorKind>(kind));
}

std::ostream& operator<<(std::ostream& os, const CursorKind& kind)
{
    return os << kind.str();
}

bool operator==(const CursorKind& lhs, const CursorKind& rhs)
{
    return lhs.kind() == rhs.kind();
}

bool operator==(const CursorKind& lhs, const CursorKind::Kind rhs)
{
    return lhs.kind() == rhs;
}

bool operator==(const CursorKind& lhs, const ::CXCursorKind rhs)
{
    return lhs.cxCursorKind() == rhs;
}

bool operator==(const CursorKind::Kind lhs, const CursorKind& rhs)
{
    return lhs == rhs.kind();
}

bool operator==(const ::CXCursorKind lhs, const CursorKind& rhs)
{
    return lhs == rhs.cxCursorKind();
}

bool operator!=(const CursorKind& lhs, const CursorKind& rhs)
{
    return !(lhs == rhs);
}

bool operator!=(const CursorKind& lhs, const CursorKind::Kind rhs)
{
    return !(lhs == rhs);
}

bool operator!=(const CursorKind& lhs, const ::CXCursorKind rhs)
{
    return !(lhs == rhs);
}

bool operator!=(const CursorKind::Kind lhs, const CursorKind& rhs)
{
    return !(lhs == rhs);
}

bool operator!=(const ::CXCursorKind lhs, const CursorKind& rhs)
{
    return !(lhs == rhs);
}

}

}

}

}

}

}
