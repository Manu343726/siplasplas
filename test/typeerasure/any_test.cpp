#include "mocks/invoke.hpp"
#include <siplasplas/typeerasure/any.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::std::string_literals;

TEST(AnyTest, Any64_invokeMethod)
{
    cpp::Any64 any{Class()};

    any("addIntsByValue") = &Class::addIntsByValue;
    any["i"] = &Class::i;
    any["str"] = &Class::str;

    any["i"] = 43;
    any["str"] = "hello world"s;

    EXPECT_EQ(42, any("addIntsByValue")(20, 22).get<int>());
    EXPECT_EQ(43, any["i"].get<int>());
    EXPECT_EQ("hello world", any["str"].get<std::string>());
}
