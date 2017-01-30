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
     * \brief Determine the semantic parent of the given cursor.
     *
     * The semantic parent of a cursor is the cursor that
     * semantically contains the given cursor. For many declarations,
     * the lexical and semantic parents are equivalent (the lexical
     * parent is returned by lexicalParent()).
     * They diverge when declarations or definitions are provided
     * out-of-line. For example:
     *
     * ``` cpp
     * class C {
     *     void f();
     * };
     *
     * void C::f() { }
     * ```
     *
     * In the out-of-line definition of `C::f`, the semantic parent
     * is the class `C`, of which this function is a member. The lexical
     * parent is the place where the declaration actually occurs in
     * the source code; in this case, the definition occurs in the
     * translation unit. In general, the lexical parent for a given
     * entity can change without affecting the semantics of the program,
     * and the lexical parent of different declarations of the same entity
     * may be different. Changing the semantic parent of a declaration,
     * on the other hand, can have a major impact on semantics, and
     * redeclarations of a particular entity should all have the same
     * semantic context.
     *
     * In the example above, both declarations of `C::f` have `C` as their
     * semantic context, while the lexical context of the first `C::f` is `C`
     * and the lexical context of the second `C::f` is the translation unit.
     *
     * For global declarations, the semantic parent is the translation unit.
     */
    Cursor semanticParent() const;

    /**
     * \brief Determine the lexical parent of the given cursor.
     *
     * The lexical parent of a cursor is the cursor in which the
     * given cursor was actually written. For many declarations,
     * the lexical and semantic parents are equivalent (the semantic
     * parent is returned by semanticParent()). They
     * diverge when declarations or definitions are provided out-of-line.
     * For example:
     *
     * ``` cpp
     * class C {
     *     void f();
     * };
     *
     * void C::f() { }
     * ```
     *
     * In the out-of-line definition of `C::f`, the semantic parent is
     * the class `C`, of which this function is a member. The lexical
     * parent is the place where the declaration actually occurs in
     * the source code; in this case, the definition occurs in the
     * translation unit. In general, the lexical parent for a given
     * entity can change without affecting the semantics of the program,
     * and the lexical parent of different declarations of the same
     * entity may be different. Changing the semantic parent of a
     * declaration, on the other hand, can have a major impact on
     * semantics, and redeclarations of a particular entity should all
     * have the same semantic context.
     *
     * In the example above, both declarations of `C::f` have `C` as their
     * semantic context, while the lexical context of the first `C::f`
     * is `C` and the lexical context of the second `C::f` is the translation unit.
     *
     * For declarations written in the global scope, the lexical parent
     * is the translation unit.
     */
    Cursor lexicalParent() const;

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
bool operator==(const Cursor& lhs, const Cursor& rhs);
bool operator!=(const Cursor& lhs, const Cursor& rhs);

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_CURSOR_HPP
