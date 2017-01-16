#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSORKIND_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSORKIND_HPP

#include <clang-c/Index.h>
#include "handle.hpp"
#include "string.hpp"
#include <siplasplas/reflection/parser/api/core/clang/export.hpp>

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

/**
 * \brief Provides information about the specific kind of a cursor
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT CursorKind
{
public:
#include "detail/cursorkindenum.hpp"

    /**
     * \brief Initializes a CursorKind with the given raw libclang CXCursorKind value
     */
    CursorKind(const ::CXCursorKind kind);

    /**
     * \brief Returns the cursor kind value
     */
    Kind kind() const;

    /**
     * \brief Returns an string representation of the cursor kind
     */
    cpp::constexp::ConstStringView str() const;

    /**
     * \brief Returns the underlying libclang CXCursorKind value
     */
    ::CXCursorKind cxCursorKind() const;

    /**
     * \brief Determine whether this cursor kind represents
     * an attribute
     */
    bool isAttribute() const;

    /**
     * \brief Determine whether this cursor kind represents
     * a declaration
     */
    bool isDeclaration() const;

    /**
     * \brief Determine whether this cursor kind represents
     * an expression
     */
    bool isExpression() const;

    /**
     * \brief Determine whether this cursor kind represents
     * an invalid cursor
     */
    bool isInvalid() const;

    /**
     * \brief Determine whether this cursor kind represents
     * a preprocessor entity
     */
    bool isPreprocessing() const;

    /**
     * \brief Determine whether this cursor kind represents
     * a simple reference
     */
    bool isReference() const;

    /**
     * \brief Determine whether this cursor kind represents
     * a statement
     */
    bool isStatement() const;

    /**
     * \brief Determine whether this cursor kind represents
     * a translation unit
     */
    bool isTranslationUnit() const;

    /**
     * \brief Determine whether this cursor kind represents
     * an entity not exposed by the libclang AST
     */
    bool isUnexposed() const;

private:
    Kind _kind;
};

std::ostream& operator<<(std::ostream& os, const CursorKind::Kind kind);
std::ostream& operator<<(std::ostream& os, const CursorKind& kind);

bool operator==(const CursorKind& lhs, const CursorKind& rhs);
bool operator==(const CursorKind& lhs, const CursorKind::Kind rhs);
bool operator==(const CursorKind& lhs, const ::CXCursorKind rhs);
bool operator==(const CursorKind::Kind lhs, const CursorKind& rhs);
bool operator==(const ::CXCursorKind lhs, const CursorKind& rhs);

bool operator!=(const CursorKind& lhs, const CursorKind& rhs);
bool operator!=(const CursorKind& lhs, const CursorKind::Kind rhs);
bool operator!=(const CursorKind& lhs, const ::CXCursorKind rhs);
bool operator!=(const CursorKind::Kind lhs, const CursorKind& rhs);
bool operator!=(const ::CXCursorKind lhs, const CursorKind& rhs);

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSORKIND_HPP
