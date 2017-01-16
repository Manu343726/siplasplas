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
    /**
     * \brief Identifies the onCursor() methods of the
     * Visitor class
     */
    struct Simple {};
}

/**
 * \ingroup clang
 * \brief Implements a simple AST visitor interface
 *
 * This class implements the most basic interface for AST visitation.
 * Visitor classes expose a visit() method that starts AST visitation from
 * a given cursor. Whenever an AST node is found, onCursor() method of the
 * visitor class is invoked with the current cursor (See Cursor) and the parent
 * cursor.
 *
 * The Visitor class does not implement any specific visitation algorithm but leaves
 * the control to the classes deriving from it, which may customize the algorithm by
 * returning diferent values from their overrided onCursor() methods (See Visitor::Result enum).
 *
 * Different visitors may implement new visitation algorithms based on the algorithm of a parent
 * visitor class. For this purpose, onCursor() methods are tagged with a tag type
 * as first argument identifying the specific visitor class where that visitation is defined.
 *
 * Finally, users may override the required onCursor() methods from a visitor class to manipualte the different
 * cursors returned by that visitor.
 *
 * ## TL;DR
 *
 *  - Visitor classes define onCursor() methods which are called whenever an AST node is visited
 *  - Users may implement new visitation algorithms by extending existing visitors and exposing them
 *    as new onCursor() tag-dispatched overloads.
 *  - Users may consume an AST by overriding onCursor() methods of a visitor to operate on
 *    each cursor found by the visitor algorithm.
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT Visitor
{
public:
    virtual ~Visitor() = default;

    /**
     * \brief Tag used to identify this visitor onCursor() methods
     */
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
