#include <siplasplas/constexpr/string.hpp>

//#define SIPLASPLAS_CONSTEXPR_ASSERT_RUNTIME_CHECKS_ONLY
#define SIPLASPLAS_CONSTEXPR_ASSERT_USE_GOOGLETEST
#include <siplasplas/constexpr/assert.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp::constexp;

TEST(StringTest, initializationFromStringLiteral)
{
    SIPLASPLAS_CONSTEXPR_ASSERT_EXPECT_EQ(stringLiteral("hello").size(), 6);
    SIPLASPLAS_CONSTEXPR_ASSERT_EXPECT_EQ(
        stringLiteral("hello"),
        stringLiteral("hello")
    );
}

TEST(StringTest, stringCat)
{
    SIPLASPLAS_CONSTEXPR_ASSERT_EXPECT_EQ(
        stringLiteral("hello") + stringLiteral(", world!"),
        stringLiteral("hello, world!")
    );
    SIPLASPLAS_CONSTEXPR_ASSERT_EXPECT_EQ(
        stringLiteral("hello") + stringLiteral(", ") + stringLiteral("world!"),
        stringLiteral("hello, world!")
    );
}

TEST(StringTest, conversionTest)
{
    EXPECT_EQ(stringLiteral("hello").str(), std::string("hello"));
    EXPECT_EQ(
        (stringLiteral("hello") + stringLiteral(", ") + stringLiteral("world")).str(),
        std::string("hello, world")
    );
    EXPECT_STREQ(
        stringLiteral("hello").c_str(),
        "hello"
    );
    EXPECT_STREQ(
        (stringLiteral("hello") + stringLiteral(", ") + stringLiteral("world")).c_str(),
        "hello, world"
    );
}

TEST(StringTest, stringDistanceSorting)
{
    using º
}
