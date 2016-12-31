#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSOR_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSOR_HPP

#include <clang-c/Index.h>
#include "handle.hpp"
#include "string.hpp"
#include "cursorkind.hpp"
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

class TranslationUnit;

/**
 * \ingroup clang
 * \brief A cursor representing some element in the abstract syntax tree for a translation unit.
 *
 * The cursor abstraction unifies the different kinds of entities in a program–declaration,
 * statements, expressions, references to declarations, etc.–under a single "cursor" abstraction
 * with a common set of operations. Common operation for a cursor include: getting the physical
 * location in a source file where the cursor points, getting the name associated with a cursor,
 * and retrieving cursors for any child nodes of a particular cursor.
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT Cursor
{
public:

    /**
     * \brief Initializes a Cursor with a CXCursor object returned
     * by the libclang API
     */
    Cursor(const ::CXCursor& cursor);

    /**
     * \brief Returns the spelling (short name) of the cursor
     */
    core::clang::String spelling() const;

    /**
     * \brief Returns the display name (detailed name) of the cursor
     */
    core::clang::String displayName() const;

    /**
     * \brief Returns the cursor kind (Whether it points to a namespace declaration,
     * a function definition, etc)
     */
    core::clang::CursorKind kind() const;

    /**
     * \brief Returns the translation unit where this
     */
    TranslationUnit translationUnit() const;

    /**
     * \brief Returns the raw libclang CXCursor object managed by this Cursor instance
     */
    const ::CXCursor& cxCursor() const;

    /**
     * \brief Checks whether the cursor is null. Null cursors are returned
     * by some libclang API functions for invalid cursors
     */
    bool isNull() const;

    /**
     * \brief Returns a hash value for this cursor
     */
    std::size_t hash() const;

private:
    ::CXCursor _cursor;
};

std::ostream& operator<<(std::ostream& os, const Cursor& cursor);

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSOR_HPP
