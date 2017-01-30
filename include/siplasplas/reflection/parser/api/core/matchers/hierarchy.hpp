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

/**
 * \ingroup matchers
 * \brief Checks if a root node has some ancestor matching
 * some inner matchers
 *
 * This matcher class can be used to walk up the AST searching for
 * properties of ancestor nodes. For example:
 *
 * ``` cpp
 * // Find class declarations that are direct or indirect
 * // members of classes named "foo":
 * classDecl(hasAncestor(classDecl(spelling("foo"))))
 * ```
 *
 * See hasAncestor()
 *
 * \tparam Matchers Inner matcher types
 */
template<typename... Matchers>
class AncestorMatcher : public MatcherInterface
{
public:
    /**
     * \brief Initializes an AncestorMatcher with a set
     * of inner matchers.
     */
    template<typename... InnerMatchers>
    AncestorMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    /**
     * \brief Checks if the matcher matches on a subtree
     * \returns True if the root has an ancestor matching
     * the inner matchers. False otherwise.
     */
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

/**
 * \ingroup matchers
 * \brief Checks if the parent of a node matches
 * some inner matchers
 *
 * This matcher allows checking properties on the
 * parent of a subtree. For example:
 *
 * ``` cpp
 * // Search class declaration named "foo", but only if
 * // are members of namespaces or classes named "bar":
 * classDecl(spelling("foo"), hasParent(spelling("bar")))
 * ```
 *
 * See hasParent()
 *
 * \tparam Matchers Inner matcher types
 */
template<typename... Matchers>
class ParentMatcher : public MatcherInterface
{
public:
    /**
     * \brief Initializes a ParentMatcher with a set
     * of inner matchers.
     */
    template<typename... InnerMatchers>
    ParentMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    /**
     * \brief Checks if the matcher matches on a subtree
     * \returns True if the root's parent matches the inner
     * matchers. False otherwise.
     */
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

/**
 * \ingroup matchers
 * \brief Checks if a root node has some descendant matching
 * some inner matchers
 *
 * This matcher class can be used to walk down the AST searching for
 * properties of descendant nodes. For example:
 *
 * ``` cpp
 * // Find namespaces with direct or indirect member classes
 * // named "foo":
 * namespaceDecl(hasDescendant(classDecl(spelling("foo"))))
 * ```
 *
 * See hasDescendant()
 *
 * \tparam Matchers Inner matcher types
 */
template<typename... Matchers>
template<typename... Matchers>
class DescendantMatcher : public MatcherInterface
{
public:
    /**
     * \brief Initializes a DescendantMatcher with a set
     * of inner matchers.
     */
    template<typename... InnerMatchers>
    DescendantMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    /**
     * \brief Checks if the matcher matches on a subtree
     * \returns True if the root has a descendant matching
     * the inner matchers. False otherwise.
     */
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

/**
 * \ingroup matchers
 * \brief Checks if a node has a child matching
 * some inner matchers
 *
 * This matcher allows checking properties on the
 * children of a node. For example:
 *
 * ``` cpp
 * // Search classes with a member function
 * // named "foo"
 * classDecl(has(cxxMethod(spelling("foo")))
 * ```
 *
 * See has()
 *
 * \tparam Matchers Inner matcher types
 */
template<typename... Matchers>
class ChildMatcher : public MatcherInterface
{
public:
    /**
     * \brief Initializes a ChildMatcher with a set
     * of inner matchers.
     */
    template<typename... InnerMatchers>
    ChildMatcher(InnerMatchers&&... innerMatchers) :
        _innerMatchers{Matcher::make(matchers::allOf(std::forward<InnerMatchers>(innerMatchers)...))}
    {}

    /**
     * \brief Checks if the matcher matches on a subtree
     * \returns True if the root node has a child matching
     * the inner matchers. False otherwise.
     */
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

/**
 * \ingroup matchers
 * \brief Checks if a node has an ancestor matching
 * the given matchers
 *
 * See AncestorMatcher
 */
template<typename... Matchers>
AncestorMatcher<std::decay_t<Matchers>...> hasAncestor(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

/**
 * \ingroup matchers
 * \brief Checks if the parent of a node matches
 * the given matchers
 *
 * See ParentMatcher
 */
template<typename... Matchers>
ParentMatcher<std::decay_t<Matchers>...> hasParent(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

/**
 * \ingroup matchers
 * \brief Checks if a node has a descendant matching
 * the given matchers
 *
 * See DescendantMatcher
 */
template<typename... Matchers>
DescendantMatcher<std::decay_t<Matchers>...> hasDescendant(Matchers&&... matchers)
{
    return { std::forward<Matchers>(matchers)... };
}

/**
 * \ingroup matchers
 * \brief Checks if a node has a child matching
 * the given matchers
 *
 * See ChildMatcher
 */
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
