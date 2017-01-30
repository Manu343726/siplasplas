#include <siplasplas/reflection/parser/api/core/matchers/kind.hpp>

using namespace cpp::reflection::parser::api::core;
using namespace cpp::reflection::parser::api::core::clang;
using namespace cpp::reflection::parser::api::core::matchers;

bool KindMatcher::matches(const Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder)
{
    if(root.kind() == _kind)
    {
        return _innerMatchers.matches(root, boundCursors, finder);
    }
    else
    {
        return false;
    }
}
