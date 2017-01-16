#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_FILTEREDVISITOR_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_FILTEREDVISITOR_HPP

#include "visitor.hpp"
#include <siplasplas/utility/memberfunctor.hpp>

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
 * \brief Default tag used to identify onCusor()
 * methods of a FilteredVisitor class.
 *
 * \tparam Predicate Predicate functor type used
 * by the visitor
 */
template<typename Predicate>
struct Filter {};

}

/**
 * \brief Implements a filtered visitation algorithm
 * on top of an existing visitor
 *
 * A FilteredVisitor filters the cursors visited by a given
 * visitor following an specific criteria. The onCursor()
 * overload from the existing visitor is hidden and a custom
 * onCusor() overload is exposed instead (Note this means the original
 * visitation algorithm is no longer accesible by derived visitors).
 *
 * \tparam Predicate Functor type implementing a predicate of signature
 *         `bool(Cursor current, Cursor parent)` implementing the filter
 *         criteria. The FilteredVisitor "visits" (i.e invokes its
 *         onCursor() overloads) a cursor only if the predicate returns true.
 *         If the predicate returns false, onIgnoredCursor() is invoked instead.
 *
 * \tparam Visitor Original visitor class
 * \tparam VisitorTag Tag used to identity this visitor. `visitor_tags::Filter<Predicate>`
 *         by default.
 */
template<
    typename Predicate,
    typename Visitor = core::clang::Visitor,
    typename VisitorTag = core::clang::visitor_tags::Filter<Predicate>
>
class FilteredVisitor : public Visitor, cpp::MemberFunctor<Predicate>
{
    using Predicate_ = cpp::MemberFunctor<Predicate>;

public:
    /**
     * \brief Tag used to identify this visitor onCursor() methods
     */
    using Tag = VisitorTag;

    FilteredVisitor(Predicate predicate = Predicate()) :
        Predicate_(predicate)
    {}

    /**
     * \brief Invoked whenver a cursor coming from the original visitation
     * algorithm is accepted.
     *
     * \param Tag Tag identifying this FilteredVisitor class
     * \param current Current cursor being visited
     * \param parent Parent of the current cursor
     *
     * \return Visitor::Result::Continue to continue the visitation normally.
     * Visitor::Result::Break if the visitation must be aborted.
     */
    virtual core::clang::Visitor::Result onCursor(Tag, const Cursor& current, const Cursor& parent) const
    {
        return core::clang::Visitor::Result::Break;
    }

    /**
     * \brief Invoked whenver a cursor coming from the original visitation
     * algorithm is accepted.
     *
     * \param Tag Tag identifying this FilteredVisitor class
     * \param current Current cursor being visited
     * \param parent Parent of the current cursor
     *
     * \return Visitor::Result::Continue to continue the visitation normally.
     * Visitor::Result::Break if the visitation must be aborted.
     */
    virtual core::clang::Visitor::Result onCursor(Tag, const Cursor& current, const Cursor& parent)
    {
        return core::clang::Visitor::Result::Break;
    }

    /**
     * \brief Invoked whenver a cursor coming from the original visitation
     * algorithm discarded.
     *
     * \param Tag Tag identifying this FilteredVisitor class
     * \param current Current cursor being visited
     * \param parent Parent of the current cursor
     *
     * \return Visitor::Result::Continue to continue the visitation normally.
     * Visitor::Result::Break if the visitation must be aborted.
     */
    virtual core::clang::Visitor::Result onIgnoredCursor(Tag, const Cursor& current, const Cursor& parent) const
    {
        return core::clang::Visitor::Result::Continue;
    }

    /**
     * \brief Invoked whenver a cursor coming from the original visitation
     * algorithm discarded.
     *
     * \param Tag Tag identifying this FilteredVisitor class
     * \param current Current cursor being visited
     * \param parent Parent of the current cursor
     *
     * \return Visitor::Result::Continue to continue the visitation normally.
     * Visitor::Result::Break if the visitation must be aborted.
     */
    virtual core::clang::Visitor::Result onIgnoredCursor(Tag, const Cursor& current, const Cursor& parent)
    {
        return core::clang::Visitor::Result::Continue;
    }

private:
    core::clang::Visitor::Result onCursor(typename Visitor::Tag, const Cursor& current, const Cursor& parent) const override final
    {
        if(Predicate_::invoke(current, parent))
        {
            return onCursor(Tag(), current, parent);
        }
        else
        {
            return onIgnoredCursor(Tag(), current, parent);
        }
    }

    core::clang::Visitor::Result onCursor(typename Visitor::Tag, const Cursor& current, const Cursor& parent) override final
    {
        if(Predicate_::invoke(current, parent))
        {
            return onCursor(Tag(), current, parent);
        }
        else
        {
            return onIgnoredCursor(Tag(), current, parent);
        }
    }
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_FILTEREDVISITOR_HPP
