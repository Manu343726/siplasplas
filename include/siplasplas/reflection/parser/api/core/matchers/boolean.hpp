#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_BOOLEAN_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_BOOLEAN_HPP

#include "variadic.hpp"

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

/**
 * \ingroup matchers
 * \brief A matcher that matches any subtree
 */
class AnythingMatcher : public MatcherInterface
{
public:
    bool matches(const core::clang::Cursor& cursor, BoundCursors& boundCursors, AstMatchFinder& finder) override final;
};

/**
 * \ingroup matchers
 * \brief Matches unless any of its inner matchers match.
 */
template<typename... Matchers>
class UnlessMatcher : public MatcherInterface
{
public:
    template<typename... Matchers_>
    UnlessMatcher(Matchers_&&... innerMatchers) :
        _innerMatchers{std::forward<Matchers_>(innerMatchers)...}
    {}

    bool matches(const core::clang::Cursor& cursor, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return !cpp::allOfTuple([&](const auto& matcher)
        {
            // Kill me
            auto& nonConstMatcher = const_cast<std::decay_t<decltype(matcher)>&>(matcher);

            // Do not bind cursors in this subtree, since when the inner matcher
            // matches (so an IdMatcher would perform a bind), this matcher should
            // not match.
            BoundCursors dummy;
            return nonConstMatcher.matches(cursor, dummy, finder);
        }, _innerMatchers);
    }

private:
    std::tuple<Matchers...> _innerMatchers;
};

/**
 * \ingroup matchers
 * \brief Implements a variadic matcher that returns the
 * logical or of its inner matchers
 */
template<typename... Matchers>
using AnyMatcher = VariadicMatcher<std::logical_or<>, false, Matchers...>;

/**
 * \ingroup matchers
 * \brief Implements a variadic matcher that returns the
 * logical and of its inner matchers
 */
template<typename... Matchers>
using AllMatcher = VariadicMatcher<std::logical_and<>, true, Matchers...>;


/**
 * \ingroup matchers
 * \brief Matches any given subtree
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT AnythingMatcher anything();

/**
 * \ingroup matchers
 * \brief Matches unless any of its inner matchers match.
 */
template<typename... Matchers>
UnlessMatcher<std::decay_t<Matchers>...> unless(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

/**
 * \ingroup matchers
 * \brief Returns an AnyMatcher of the given matchers
 */
template<typename... Matchers>
AnyMatcher<std::decay_t<Matchers>...> anyOf(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

/**
 * \ingroup matchers
 * \brief Returns an AllMatcher of the given matchers
 */
template<typename... Matchers>
AllMatcher<std::decay_t<Matchers>...> allOf(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_BOOLEAN_HPP
