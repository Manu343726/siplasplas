#include "matcher.hpp"

using namespace cpp::constexp;
using namespace cpp::reflection::parser::api::core;
using namespace cpp::reflection::parser::api::core::clang;

void BoundCursors::bind(ConstStringView name, const Cursor& cursor)
{
    auto result = _cursors.insert({name.str(), cursor});

    if(!result.second)
    {
        const Cursor& alreadyBound = result.first->second;

        throw cpp::exception<std::runtime_error>(
            "A cursor is already bound to the name '{}' ({})",
            name,
            alreadyBound
        );
    }
}

const Cursor& BoundCursors::get(ConstStringView name) const
{
    auto it = _cursors.find(name.str());

    if(it != _cursors.end())
    {
        return it->second;
    }
    else
    {
        throw cpp::exception<std::out_of_range>(
            "No cursor bound with name '{}'",
            name
        );
    }
}



Matcher::Matcher(std::shared_ptr<MatcherInterface> impl) :
    _impl{impl}
{}

bool Matcher::matches(const Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder)
{
    return _impl->matches(root, boundCursors, finder);
}



bool AstMatchFinder::matchesParent(
    Matcher& matcher,
    const Cursor& root,
    BoundCursors& boundCursors
)
{
    return matchesAncestor(matcher, root, boundCursors, 1);
}

bool AstMatchFinder::matchesChild(
    Matcher& matcher,
    const Cursor& root,
    BoundCursors& boundCursors
)
{
    return matchesDescendant(matcher, root, boundCursors, 1);
}
