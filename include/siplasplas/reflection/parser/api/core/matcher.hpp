#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHER_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHER_HPP

#include "clang/cursor.hpp"
#include <siplasplas/constexpr/stringview.hpp>
#include <siplasplas/constexpr/arrayview.hpp>
#include <siplasplas/reflection/parser/api/core/export.hpp>
#include <vector>

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
 * \brief Registry of named cursors that matched a given matcher
 *
 * This class implements a name -> cursor maps to identify cursors
 * that matched a given matcher. Users may bind names to different
 * sub-matchers, and the matching system registers cursors matching
 * these binded matchers here, so the different components of the
 * match can be retreived easily.
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT BoundCursors
{
public:
    /**
     * \brief Registers a matching cursor with the given name
     */
    void bind(cpp::constexp::ConstStringView name, const core::clang::Cursor& cursor);

    /**
     * \brief Returns the matching cursor bound to the given name
     *
     * If there are multiple cursors bound to the given name, returns the first one
     */
    const core::clang::Cursor& get(cpp::constexp::ConstStringView name) const;

    /**
     * \brief Returns the set of cursors bound to a given name
     */
    cpp::constexp::ConstArrayView<core::clang::Cursor> getAll(cpp::constexp::ConstStringView name) const;

private:
    std::unordered_map<std::string, std::vector<core::clang::Cursor>> _cursors;
};

class AstMatchFinder;

/**
 * \ingroup core
 * \brief Implements the minimal interface of an AST matcher
 *
 * A Matcher class defines the properties of a certain AST
 * subtree. Given a cursor, the job of a matcher is to identify
 * if the subtree rooted at that cursor matches the properties
 * represented. For example:
 *
 * ``` cpp
 * class FooParentMatcher : public MatcherInterface
 * {
 * public:
 *     bool matches(const core::clang::Cursor& root) override
 *     {
 *         return root.semanticParent().spelling() == "Foo";
 *     }
 * };
 * ```
 *
 * The matcher defined above matches subtrees comming from an
 * ancestor node named "Foo". Given:
 *
 * ``` cpp
 * namespace Bar
 * {
 *     namespace Foo
 *     {
 *         class Class
 *         {
 *         };
 *
 *         void f();
 *     }
 *
 *     class Class
 *     {
 *     };
 * }
 * ```
 *
 * a FooParentMatcher would match at `Bar::Foo::Class` and `Bar::Foo::f`.
 *
 * Matchers are useful to search for certain constructs in the AST,
 * instead of branching and searching manually in the AST with visitors
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT MatcherInterface
{
public:
    /**
     * \brief Tests if the matcher matches on the given subtree rooted at
     * \p root
     *
     * \param Root Cursor pointing to the root of the AST subtree where to check
     * the match against.
     * \param Finder A AstMatchFinder instance that allows the matcher to
     * traverse the AST to find matches on ancestors or descendants.
     *
     * \returns True if matches on \p root, false otherwise.
     */
    virtual bool matches(
        const core::clang::Cursor& root,
        BoundCursors& boundCursors,
        AstMatchFinder& finder
    ) = 0;
};

/**
 * \ingroup core
 * \brief Implements a type erased matcher
 *
 * Matcher class wraps a MatcherInterface instance in a common
 * class that type-erases the instance providing a common access
 * point and storage for all kind of matchers.
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT Matcher
    : public MatcherInterface
{
public:
    /**
     * \brief Initializes a type-erased Matcher with a given
     * MatcherInterface implementation
     */
    Matcher(std::shared_ptr<MatcherInterface> impl);

    /**
     * \brief Returns the given matcher as a type-erased Matcher
     * instance.
     */
    template<typename MatcherType>
    static Matcher make(MatcherType&& matcher)
    {
        static_assert(std::is_base_of<MatcherInterface, std::decay_t<MatcherType>>::value,
            "The matcher to type-erase must implement the MatcherInterface interface");

        return { std::make_shared<std::decay_t<MatcherType>>(
            std::forward<MatcherType>(matcher)
        ) };
    }

    /**
     * \brief Checks if the matcher matches on a given subtree. Forwards
     * to the underlying matcher match() method.
     */
    bool matches(
        const core::clang::Cursor& current,
        BoundCursors& boundCursors,
        AstMatchFinder& finder
    ) override final;

private:
    std::shared_ptr<MatcherInterface> _impl;
};

/**
 * \ingroup core
 * \brief Provides an interface to allow matchers traverse the AST
 * to search for matches
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_EXPORT AstMatchFinder
{
public:
    /**
     * \brief Checks if a given matcher matches on an ancestor subtree
     * of the given subtree.
     *
     * This function tries to find a match on the ancestors of the given subtree.
     * To do so, a visitation goes upwards the AST from parent to parent checking
     * running the matcher on each level of the hierarchy. \p maxDepth controlls the
     * maximum number of upper levels that will be checked.
     *
     * \param matcher Matcher to test against ancestors of \p current
     * \param current Cursor pointing to the root of the given subtree
     * \param maxDepth Maximum of parent levels that must be tested
     *
     * \returns True if a match is found up in the AST hierarchy. False otherwise.
     */
    virtual bool matchesAncestor(
        Matcher& matcher,
        const core::clang::Cursor& current,
        BoundCursors& boundCursors,
        std::size_t maxDepth
    ) = 0;

    /**
     * \brief Similar to matchesAncestor(), but checks in the direct ancestor
     * of the root (the parent) only.
     */
    bool matchesParent(
        Matcher& matcher,
        const core::clang::Cursor& root,
        BoundCursors& boundCursors
    );

    /**
     * \brief Checks if a given matcher matches on a descendant of the
     * given AST node
     *
     * This function tries to find a match on any of the descendant nodes of the
     * given AST node. To do so, a recursive descendant visitation is performed until
     * a match is found, or if the visitation goes more than \p maxDepth levels down
     * the AST.
     *
     * \param matcher Matcher to test against the descendants.
     * \param root Cursor pointing to the AST node where to start
     * the search.
     * \maxDepth The maximum levels to go down through the AST searching for matches
     *
     * \returns True if a match is found in any of the descendants of \p root, false
     * otherwise.
     */
    virtual bool matchesDescendant(
        Matcher& matcher,
        const core::clang::Cursor& root,
        BoundCursors& boundCursors,
        std::size_t maxDepth
    ) = 0;

    /**
     * \brief Similar to matchesDescendant(), but checks the direct descendants (children)
     * of the root node only.
     */
    bool matchesChild(
        Matcher& matcher,
        const core::clang::Cursor& root,
        BoundCursors& boundCursors
    );
};

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHER_HPP
