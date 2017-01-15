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

template<typename Predicate>
struct Filter {};

}

template<typename Predicate, typename Visitor = core::clang::Visitor, typename VisitorTag = core::clang::visitor_tags::Filter<Predicate>>
class FilteredVisitor : public Visitor, cpp::MemberFunctor<Predicate>
{
    using Predicate_ = cpp::MemberFunctor<Predicate>;

public:
    using Tag = VisitorTag;

    FilteredVisitor(Predicate predicate = Predicate()) :
        Predicate_(predicate)
    {}

    virtual core::clang::Visitor::Result onCursor(Tag, const Cursor& current, const Cursor& parent) const
    {
        return core::clang::Visitor::Result::Break;
    }

    virtual core::clang::Visitor::Result onCursor(Tag, const Cursor& current, const Cursor& parent)
    {
        return core::clang::Visitor::Result::Break;
    }

    virtual core::clang::Visitor::Result onIgnoredCursor(Tag, const Cursor& current, const Cursor& parent) const
    {
        return core::clang::Visitor::Result::Continue;
    }

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
