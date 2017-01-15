#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_RECURSIVEVISITOR_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_RECURSIVEVISITOR_HPP

#include "visitor.hpp"

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
    struct Recursive {};
}

/**
 * \ingroup clang
 * \brief Implements a recursive AST visitor interface
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT RecursiveVisitor : public Visitor
{
public:
    using Tag = core::clang::visitor_tags::Recursive;

    /**
     * \brief Callback invoked each time a cursor is visited. Aborts visitation by
     * returning Result::Break by default
     *
     * \param current Current cursor being visited
     * \param parent Parent of the current cursor
     *
     * \returns A Result value to indicate whether the traversal of the AST must
     * be aborted or continued
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
     * be aborted or continued
     */
    virtual Result onCursor(Tag, const Cursor& current, const Cursor& parent);

private:
    Result onCursor(Visitor::Tag, const Cursor& current, const Cursor& parent) const override final;
    Result onCursor(Visitor::Tag, const Cursor& current, const Cursor& parent)       override final;
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_RECURSIVEVISITOR_HPP
