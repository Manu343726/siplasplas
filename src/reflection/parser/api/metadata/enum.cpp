#include "enum.hpp"

using namespace cpp::reflection::parser::api::metadata;
using namespace cpp::reflection::parser::api::core::matchers;

Matcher Enum::matcher()
{
    static Matcher matcher = enum
}
