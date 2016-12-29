#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSORKIND_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSORKIND_HPP

#include <clang-c/Index.h>
#include "handle.hpp"
#include "string.hpp"
#include <type_safe/strong_typedef.hpp>
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

class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT CursorKind
    : public type_safe::strong_typedef<CursorKind, ::CXCursorKind>
    , public type_safe::strong_typedef_op::equality_comparison<::CXCursorKind>
    , public type_safe::strong_typedef_op::mixed_equality_comparison<CursorKind, ::CXCursorKind>
{
public:
    using type_safe::strong_typedef<CursorKind, ::CXCursorKind>::strong_typedef;

    CursorKind(const ::CXCursorKind kind) :
        type_safe::strong_typedef<CursorKind, ::CXCursorKind>{kind}
    {}

    /**
     * \brief Returns an string representation of the cursor kind
     */
    core::clang::String str() const;

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
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSORKIND_HPP
