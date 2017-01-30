#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KIND_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KIND_HPP

#include <siplasplas/constexpr/stringview.hpp>
#include <siplasplas/reflection/parser/api/core/matcher.hpp>
#include <siplasplas/reflection/parser/api/core/export.hpp>
#include "boolean.hpp"
#include "properties.hpp"

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
 * \brief Matches cursors of a given kind. Implicitly works as
 * a AllOfMatcher for its inner matchers (If any).
 */
class KindMatcher : public MatcherInterface
{
public:
    /**
     * \brief Initializes a KindMatcher with the given kind and inner matchers
     */
    template<typename... Matchers>
    KindMatcher(const core::clang::CursorKind::Kind kind, Matchers&&... innerMatchers) :
        _kind{kind},
        _innerMatchers{Matcher::make(matchers::anyOf(std::forward<Matchers>(innerMatchers)...))}
    {}

    /**
     * \brief Checks if the matcher matches on a given cursor.
     *
     * \returns True if the cursor has the requested kind, false otherwise.
     */
    bool matches(const core::clang::Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder) override final;

private:
    core::clang::CursorKind::Kind _kind;
    Matcher _innerMatchers;
};

/**
 * \ingroup matchers
 * \brief Matches cursors of a given kind. Implicitly works as
 * a AllOfMatcher for its inner matchers (If any).
 */
template<typename... Matchers>
KindMatcher withKind(const core::clang::CursorKind::Kind kind, Matchers&&... matchers)
{
    return { kind, std::forward<Matchers>(matchers)... };
}

/**
 * \ingroup matchers
 * \brief Defines a kind matcher API
 *
 * \param matcher Name of the API function
 * \param kind Cursor kind to match against
 */
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER(matcher, kind)            \
    template<typename... Matchers>                                                           \
    KindMatcher matcher(Matchers&&... matchers)                                              \
    {                                                                                        \
        return { core::clang::CursorKind::Kind::kind, std::forward<Matchers>(matchers)... }; \
    }

/**
 * \ingroup matchers
 * \brief Similar to SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER(),
 * but allows passing custom inner matchers
 *
 * \param matcher Name of the API function
 * \param kind Cursor kind to match against
 */
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER_EX(matcher, kind, ...)                 \
    template<typename... Matchers>                                                                        \
    KindMatcher matcher(Matchers&&... matchers)                                                           \
    {                                                                                                     \
        return { core::clang::CursorKind::Kind::kind, __VA_ARGS__, std::forward<Matchers>(matchers)... }; \
    }

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KIND_HPP
