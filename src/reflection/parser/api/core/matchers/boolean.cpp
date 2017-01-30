#include <siplasplas/reflection/parser/api/core/matchers/boolean.hpp>

using namespace cpp::reflection::parser::api::core;
using namespace cpp::reflection::parser::api::core::clang;
using namespace cpp::reflection::parser::api::core::matchers;

bool AnythingMatcher::matches(const Cursor& root, BoundCursors& boundCursors, AstMatchFinder& finder)
{
    return true;
}

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

AnythingMatcher anything()
{
    return {};
}

}

}

}

}

}

}

