#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_VARIADICMATCHERS_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_VARIADICMATCHERS_HPP

#include <siplasplas/utility/memberfunctor.hpp>
#include <siplasplas/utility/fusion.hpp>
#include <siplasplas/reflection/parser/api/core/matcher.hpp>
#include <siplasplas/reflection/parser/api/core/export.hpp>

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
 * \brief Implements a matcher acting as the aggregation of the given matchers,
 * using a given operation.
 *
 * \tparam Op Operation used to aggregate the result of the multiple matchers,
 * must be a callable of signature `bool(MatcherInterface&, MatcherInterface&)`
 * \tparam Seed Initial result of the matcher
 * \tparam Matchers Set of matcher types
 */
template<typename Op, bool Seed, typename... Matchers>
class VariadicMatcher
    : public MatcherInterface
    , public cpp::MemberFunctor<Op>
{
using Functor = cpp::MemberFunctor<Op>;

public:
    /**
     * \brief Initializes a variadic matcher
     *
     * \param op Aggregation operation
     * \param matchers Matchers to be aggregated
     */
    template<typename... Matchers_, typename =
        std::enable_if_t<sizeof...(Matchers_) == sizeof...(Matchers)>
    >
    VariadicMatcher(Op op, Matchers_&&... matchers) :
        Functor{op},
        _matchers{std::forward<Matchers_>(matchers)...}
    {}

    /**
     * \brief Initializes a variadic matcher
     *
     * The operation is default-initialized
     *
     * \param matchers Matchers to be aggregated
     */
    template<typename... Matchers_, typename =
        std::enable_if_t<sizeof...(Matchers_) == sizeof...(Matchers)>
    >
    VariadicMatcher(Matchers_&&... matchers) :
        VariadicMatcher{Op(), std::forward<Matchers_>(matchers)...}
    {}

    /**
     * \brief Checks if a given subtree matches on the set of matchers
     * following the aggregation operation
     */
    bool matches(const core::clang::Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return cpp::foldTuple([&, this](bool previous, const auto& matcher)
        {
            // Kill me
            auto& noConstMatcher = const_cast<std::decay_t<decltype(matcher)>&>(matcher);

            return this->invoke(previous, noConstMatcher.matches(root, boundCursors, finder));
        }, Seed, _matchers);
    }

private:
    std::tuple<Matchers...> _matchers;
};

template<typename Op, bool Seed>
class VariadicMatcher<Op, Seed> : public MatcherInterface
{
public:
    VariadicMatcher() = default;
    VariadicMatcher(Op op) : VariadicMatcher{} {}

    bool matches(const core::clang::Cursor& cursor, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return true;
    }
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_VARIADICMATCHERS_HPP
