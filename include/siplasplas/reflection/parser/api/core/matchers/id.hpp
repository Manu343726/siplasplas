#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_ID_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_ID_HPP

#include <siplasplas/constexpr/stringview.hpp>
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
 * \brief Allows a given matcher to bind its matching cursors
 * to a given id
 *
 * This matcher allows introspection of a complex match by giving
 * names to the different inner matchers of the matcher. When a match is
 * found, cursors matching on the different parts of the matchers can be
 * queried by their bind id. For example:
 *
 * ``` cpp
 * class MyCallback : public MatcherCallback
 * {
 * public:
 *     void onMatch(const MatcherCallback::MatchResult& result) override final
 *     {
 *         std::cout << "Child found: " << result.boundCursors.get("child");
 *     }
 * };
 *
 * MatchFinder finder;
 * MyCallback callback;
 *
 * finder.addMatcher(
 *     namespaceDecl(
 *         id("child", has(classDecl())) // Class decl cursors found will be named "child"
 *     ),
 *     callback
 *  );
 * ```
 *
 * \tparam Matcher
 */
template<typename Matcher>
class IdMatcher : public MatcherInterface
{
public:
    /**
     * \brief Binds a matcher to the given id
     */
    IdMatcher(cpp::constexp::ConstStringView id, Matcher&& matcher) :
        _matcher{std::move(matcher)},
        _id{id.str()}
    {}

    /**
     * \brief Binds a matcher to the given id
     */
    IdMatcher(cpp::constexp::ConstStringView id, const Matcher& matcher) :
        _matcher{matcher},
        _id{id.str()}
    {}

    /**
     * \brief Checks if the matcher matches on a subtree. If true, the matching root
     * is binded.
     */
    bool matches(const core::clang::Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        if(_matcher.matches(root, boundCursors, finder))
        {
            boundCursors.bind(_id, root);
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    Matcher _matcher;
    std::string _id;
};

/**
 * \ingroup matchers
 * \brief Bounds a matcher to the given id
 */
template<typename Matcher>
IdMatcher<std::decay_t<Matcher>> id(cpp::constexp::ConstStringView id, Matcher&& matcher)
{
    return { id, std::forward<Matcher>(matcher) };
}

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_ID_HPP
