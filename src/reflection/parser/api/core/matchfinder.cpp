#include "matchfinder.hpp"
#include <siplasplas/reflection/parser/api/core/clang/recursivevisitor.hpp>
#include "logger.hpp"

using namespace cpp::reflection::parser::api::core;
using namespace cpp::reflection::parser::api::core::clang;
using namespace cpp::constexp;

MatchCallback::MatchResult::MatchResult(
    const Cursor& root,
    const BoundCursors& cursors,
    const Matcher& matcher
) :
    _root{root},
    _cursors{&cursors},
    _matcher{&matcher}
{}

const Cursor& MatchCallback::MatchResult::root() const
{
    return _root;
}

const BoundCursors& MatchCallback::MatchResult::cursors() const
{
    return *_cursors;
}

const Matcher& MatchCallback::MatchResult::matcher() const
{
    return *_matcher;
}

class MatchChildVisitor : public RecursiveVisitor
{
public:
    MatchChildVisitor(
        Matcher& matcher,
        BoundCursors& boundedCursors,
        AstMatchFinder& finder,
        std::size_t maxDepth
    );

    bool findMatch(const Cursor& cursor);

private:
    Matcher& _matcher;
    BoundCursors& _boundedCursors;
    AstMatchFinder& _finder;
    const std::size_t _maxDepth;
    bool _matches;
    Cursor _root;

    Visitor::Result onCursor(RecursiveVisitor::Tag, const Cursor& current, const Cursor& parent);

    void reset();
};

MatchChildVisitor::MatchChildVisitor(
    Matcher& matcher,
    BoundCursors& boundedCursors,
    AstMatchFinder& finder,
    std::size_t maxDepth
) :
    _matcher(matcher),
    _boundedCursors(boundedCursors),
    _finder(finder),
    _maxDepth{maxDepth},
    _matches{false},
    _root{Cursor::null()}
{}

bool MatchChildVisitor::findMatch(const Cursor& root)
{
    reset();

    _root = root;
    Visitor::visit(root);

    return _matches;
}

Visitor::Result MatchChildVisitor::onCursor(RecursiveVisitor::Tag, const Cursor& current, const Cursor& parent)
{
    using cpp::reflection::parser::api::core::log;
    std::size_t depth = current.distanceToAncestor(_root);

    if(current.distanceToAncestor(_root) > _maxDepth)
    {
        log().debug("MatchChildVisitor::onCursor((depth: {}) current = {}, parent = {}): Max depth ({}) reached", depth, current, parent, _maxDepth);
        // Max depth reached, abort
        return Visitor::Result::Break;
    }
    else
    {
        log().debug("MatchChildVisitor::onCursor((depth: {}) current = {}, parent = {})", depth, current, parent);

        if(_matcher.matches(current, _boundedCursors, _finder))
        {
            _matches = true;
        }

        return Visitor::Result::Continue;
    }
}

void MatchChildVisitor::reset()
{
    _matches = false;
    RecursiveVisitor::resetDepth();
    _root = Cursor::null();
}

//
// This class visits an AST trying to match
// each visited subtree against a given matcher.
// This viistor is spawned by MatchFinder class as
// the outermost visitor of the system, but note
// matchers are free to spawn other visitors on
// visited cursors in order to find inner matchers
// up or down the AST.
//
// The object also acts as a match finder for the matcher
// passed to it.
class MatchingAstVisitor
    : public RecursiveVisitor
    , public AstMatchFinder
{
public:
    MatchingAstVisitor(ArrayView<MatchFinder::MatchFinder::MatcherWithCallback> matchers);

private:
    ArrayView<MatchFinder::MatcherWithCallback> _matchers;

    void match(const Cursor& root);
    VisitorResult onCursor(visitor_tags::Recursive, const Cursor& current, const Cursor& parent) override final;
    bool matchesDescendant(Matcher& matcher, const Cursor& root, BoundCursors& boundCursors, std::size_t maxDepth) override final;
    bool matchesAncestor(Matcher& matcher, const Cursor& root, BoundCursors& boundCursor, std::size_t maxDepth) override final;
};

MatchingAstVisitor::MatchingAstVisitor(ArrayView<MatchFinder::MatcherWithCallback> matchers) :
    _matchers{matchers}
{}

void MatchingAstVisitor::match(const Cursor& root)
{
    for(auto& matcherAndCallback : _matchers)
    {
        auto& matcher = matcherAndCallback.first;
        auto& callback = *matcherAndCallback.second;

        BoundCursors boundCursors;

        if(matcher.matches(root, boundCursors, *this))
        {
            callback.onMatch({root, boundCursors, matcher});
        }
    }
}

VisitorResult MatchingAstVisitor::onCursor(RecursiveVisitor::Tag, const Cursor& current, const Cursor& parent)
{
    using cpp::reflection::parser::api::core::log;
    log().debug("MatchingAstVisitor::onCursor(current = {}", current);
    // Try to find a match in the currently visited node
    // Note this may spawn new recursive visitors to find
    // inner matches in the AST
    match(current);

    return VisitorResult::Continue;
}

// Invoked when an parent matcher requests an inner match against
// children or descendant of the currently visited cursor (Where the
// inner matcher for that match is matcher param bellow)
bool MatchingAstVisitor::matchesDescendant(Matcher& matcher, const Cursor& root, BoundCursors& boundCursors, std::size_t maxDepth)
{
    MatchChildVisitor visitor{matcher, boundCursors, *this, maxDepth};
    return visitor.findMatch(root);
}

// Invoked when an parent matcher requests an inner match against
// parent or ancestor of the currently visited cursor (Where the
// inner matcher for that match is matcher param bellow)
bool MatchingAstVisitor::matchesAncestor(Matcher& matcher, const Cursor& root, BoundCursors& boundCursors, std::size_t maxDepth)
{
    if(maxDepth == 0)
    {
        return false;
    }
    else
    {
        Cursor parent = root.semanticParent();

        if(parent.isNull())
        {
            // Root has no parent, abort
            return false;
        }
        else
        {
            return matcher.matches(parent, boundCursors, *this);
        }
    }
}



void MatchFinder::addMatcher(const Matcher& matcher, MatchCallback& callback)
{
    _matchers.emplace_back(std::make_pair(matcher, &callback));
}

void MatchFinder::matchAst(const TranslationUnit& tu)
{
    MatchingAstVisitor visitor{ArrayView<MatcherWithCallback>{&_matchers[0], &_matchers[_matchers.size()]}};
    visitor.visit(tu.cursor());
}
