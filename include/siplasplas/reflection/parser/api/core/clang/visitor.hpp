#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_VISITOR_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_VISITOR_HPP

#include "cursor.hpp"
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

namespace visitor_tags
{
    struct Simple {};
}

/**
 * \ingroup clang
 * \brief Implements an AST visitor interface
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT Visitor
{
public:
    virtual ~Visitor() = default;

    using Tag = core::clang::visitor_tags::Simple;

    /**
     * \brief Describes how the traversal of the children of a particular
     * cursor should proceed after visiting a particular child cursor.
     */
    enum class Result
    {
        Break    = ::CXChildVisit_Break,    ///< Terminates the cursor trasversal
        Continue = ::CXChildVisit_Continue, ///< Continues the cursor traversal with the next sibling of the cursor just visited, without visiting its children
        Recurse  = ::CXChildVisit_Recurse   ///< Recursively traverse the children of this cursor, using the same visitor and client data
    };

    /**
     * \brief Starts traversing the AST from the given root cursor
     *
     * \param root Cursor where the AST traversal starts
     *
     * \returns True if the traversal was aborted prematurely by returning
     * Result::Break. False otherwise
     */
    bool visit(const Cursor& root) const;

    /**
     * \brief Starts traversing the AST from the given root cursor
     *
     * \param root Cursor where the AST traversal starts
     *
     * \returns True if the traversal was aborted prematurely by returning
     * Result::Break. False otherwise
     */
    bool visit(const Cursor& root);

    /**
     * \brief Callback invoked each time a cursor is visited. Aborts visitation by
     * returning Result::Break by default
     *
     * \param current Current cursor being visited
     * \param parent Parent of the current cursor
     *
     * \returns A Result value to indicate whether the traversal of the AST must
     * be aborted, continue through siblings, or deep recursively
     */
    virtual Result onCursor(Tag, const Cursor& current, const Cursor& parent) const;

    /**
     * \brief Callback invoked each time a cursor is visited. Aborts visitation by
     * returning Result::Break by default
     *
     * \param current Current cursor being visited
     * \param parent Parent of the current cursor
     *
     * \returns A Result value to indicate whether the traversal of the AST must
     * be aborted, continue through siblings, or deep recursively
     */
    virtual Result onCursor(Tag, const Cursor& current, const Cursor& parent);
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_VISITOR_HPP
