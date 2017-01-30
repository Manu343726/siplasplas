#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_VISITORINTERFACE_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_VISITORINTERFACE_HPP

#include "visitor.hpp"
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
 * \ingroup clang
 * \brief Defines a common type-erased interface for visitors
 *
 * This class defines a common interface for visitor classes so these
 * can be easily consumed by user APIs. The class removes the visitor
 * tag and provides a common onCursor() method.
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT VisitorInterface
{
public:
    virtual ~VisitorInterface() = default;

    /**
     * \brief Starts traversing the AST from the given root cursor
     *
     * \param root Cursor where the AST traversal starts
     *
     * \returns True if the traversal was aborted prematurely by returning
     * Result::Break. False otherwise
     */
    virtual bool visit(const Cursor& root) const = 0;

    /**
     * \brief Starts traversing the AST from the given root cursor
     *
     * \param root Cursor where the AST traversal starts
     *
     * \returns True if the traversal was aborted prematurely by returning
     * Result::Break. False otherwise
     */
    virtual bool visit(const Cursor& root) = 0;

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
    virtual Visitor::Result onCursor(const Cursor& current, const Cursor& parent) const;

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
    virtual Visitor::Result onCursor(const Cursor& current, const Cursor& parent);

    /**
     * \brief Type erases a given visitor class by automatically implementing
     * the VisitorInterface from the given visitor.
     *
     * Instances of this template inherit both from the Visitor class given and
     * the VisitorInterface class, so despite the specific onVisitor() method from the
     * Visitor will be hidden for the user, other features offered by the visitor
     * API are still accessible. At the same time, the resulting class will implement
     * the VisitorInterface, so it will be usable in user API expecting a simplified
     * visitor interface.
     *
     * **NOTE:** Only the onCursor() overload selected by the Tag parameter of the
     * template will be overrided and hidden. All other overloads (if any) will be part
     * of the class interface.
     *
     * \tparam Visitor Visitor class to type-erase
     * \tparam Tag Tag that identifies the onCursor() overload to base VisitorInterface::onCursor()
     * on. Defaults to the tag of the given cursor class.
     */
    template<typename Visitor, typename Tag = typename Visitor::Tag>
    class Make;
};

template<typename Visitor, typename Tag>
class VisitorInterface::Make : private Visitor, public VisitorInterface
{
public:
    using Visitor::Visitor;

    bool visit(const Cursor& cursor) const override final
    {
        return Visitor::visit(cursor);
    }

    bool visit(const Cursor& cursor)       override final
    {
        return Visitor::visit(cursor);
    }

private:
    VisitorResult onCursor(Tag, const Cursor& current, const Cursor& parent) const override final
    {
        return static_cast<const VisitorInterface*>(this)->onCursor(current, parent);
    }

    VisitorResult onCursor(Tag, const Cursor& current, const Cursor& parent)       override final
    {
        return static_cast<VisitorInterface*>(this)->onCursor(current, parent);
    }
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_VISITORINTERFACE_HPP
