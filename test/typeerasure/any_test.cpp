#include "mocks/invoke.hpp"
#include <siplasplas/typeerasure/any.hpp>
#include <gmock/gmock.h>

using namespace ::testing;

TEST(AnyTest, Any64_invokeMethod)
{
    cpp::Any64 any{Class()};

    any("addIntsByValue") = &Class::addIntsByValue;
    any["i"] = &Class::i;

    any["i"] = 43;

    EXPECT_EQ(42, any("addIntsByValue")(20, 22).get<int>());
    EXPECT_EQ(42, any["i"].get<int>());
}
