#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHFINDER_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHFINDER_HPP

#include "matcher.hpp"
#include "clang/translationunit.hpp"

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


/**
 * \ingroup core
 * \brief Represents an action to be performed when a given matcher
 * matches on a subtree.
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT MatchCallback
{
public:
    /**
     * \brief Returns the data associated to a matching
     * AST subtree
     */
    class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT MatchResult
    {
    public:
        /**
         * \brief Initializes a match result with the associated
         * match data
         *
         * \param root Cursor pointing to the root of the AST subtree
         * that matched
         * \param cursors Set of cursors bound to the match
         * \param matcher The matcher that matched against the subtree
         */
        MatchResult(const core::clang::Cursor& root,
                    const BoundCursors& cursors,
                    const Matcher& matcher);

        /**
         * \brief Returns the root of the AST subtree that matched
         */
        const core::clang::Cursor& root() const;

        /**
         * \brief Returns the set of cursors bound to the match
         */
        const BoundCursors& cursors() const;

        /**
         * \brief Returns the matcher that matched against the subtree
         */
        const Matcher& matcher() const;

    private:
        core::clang::Cursor _root;
        const BoundCursors* _cursors;
        const Matcher* _matcher;
    };

    /**
     * \brief Callback invoked when a matcher matches on a
     * AST subtree
     *
     * \param matchResult Data associated to the match
     */
    virtual void onMatch(const MatchResult& matchResult) = 0;
};

class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT MatchFinder
{
public:
    using MatcherWithCallback = std::pair<Matcher, MatchCallback*>;

    template<typename MatcherType>
    void addMatcher(const MatcherType& matcher, MatchCallback& callback)
    {
        addMatcher(Matcher::make(matcher), callback);
    }

    void addMatcher(const Matcher& matcher, MatchCallback& callback);

    void matchAst(const core::clang::TranslationUnit& tu);

private:
    std::vector<MatcherWithCallback> _matchers;
};

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHFINDER_HPP
