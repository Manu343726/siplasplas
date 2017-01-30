#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_HIERARCHY_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_HIERARCHY_HPP

#include "boolean.hpp"
#include <climits>

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

namespace matchers
{

template<typename... Matchers>
class AncestorMatcher : public MatcherInterface
{
public:
    template<typename... InnerMatchers>
    AncestorMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    bool matches(const core::clang::Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return finder.matchesAncestor(
            _innerMatchers,
            root,
            boundCursors,
            std::numeric_limits<std::size_t>::max()
        );
    }

private:
    Matcher _innerMatchers;
};

template<typename... Matchers>
class ParentMatcher : public MatcherInterface
{
public:
    template<typename... InnerMatchers>
    ParentMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    bool matches(const core::clang::Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return finder.matchesParent(
            _innerMatchers,
            root,
            boundCursors
        );
    }

private:
    Matcher _innerMatchers;
};

template<typename... Matchers>
class DescendantMatcher : public MatcherInterface
{
public:
    template<typename... InnerMatchers>
    DescendantMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    bool matches(const core::clang::Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return finder.matchesDescendant(
            _innerMatchers,
            root,
            boundCursors,
            std::numeric_limits<std::size_t>::max()
        );
    }

private:
    Matcher _innerMatchers;
};

template<typename... Matchers>
class ChildMatcher : public MatcherInterface
{
public:
    template<typename... InnerMatchers>
    ChildMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    bool matches(const core::clang::Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return finder.matchesChild(
            _innerMatchers,
            root,
            boundCursors
        );
    }

private:
    Matcher _innerMatchers;
};

template<typename... Matchers>
AncestorMatcher<std::decay_t<Matchers>...> hasAncestor(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

template<typename... Matchers>
ParentMatcher<std::decay_t<Matchers>...> hasParent(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

template<typename... Matchers>
DescendantMatcher<std::decay_t<Matchers>...> hasDescendant(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

template<typename... Matchers>
ChildMatcher<std::decay_t<Matchers>...> has(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_HIERARCHY_HPP
