#include "mocks/valuesemantics.hpp"
#include <siplasplas/typeerasure/simpleany.hpp>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include <array>

using namespace ::cpp;
using namespace ::testing;

TEST(SimpleAnyTest, create_defaultConstructibleType_noThrow)
{
    EXPECT_NO_THROW(SimpleAny::create<DefaultConstructible>());
}

TEST(SimpleAnyTest, create_noDefaultConstructibleType_throws)
{
    EXPECT_THROW(SimpleAny::create<NoDefaultConstructible>(), std::runtime_error);
}

TEST(SimpleAnyTest, create_smallTypes_noThrow)
{
    // cpp::SimpleAny storage is of 64-8 bytes, to fit
    // n a cache line (First 8 bytes are used for
    // semantics)
    EXPECT_NO_THROW(SimpleAny::create<std::string>());
    EXPECT_NO_THROW(SimpleAny::create<std::vector<std::string>>());
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 4>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 8>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 16>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 32>>()));
}

TEST(SimpleAnyTest, createWithCustomArgs_smallTypes_noThrow)
{
    // cpp::SimpleAny storage is of 64-8 bytes, to fit
    // n a cache line (First 8 bytes are used for
    // semantics)
    EXPECT_NO_THROW(SimpleAny::create<std::string>("hello, world!"));
    EXPECT_NO_THROW(SimpleAny::create<std::vector<std::string>>(42, "hello, world!"));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 4>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 8>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 16>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 32>>()));
}

TEST(SimpleAnyTest, createWithCustomArgs_bigTypes_throw)
{
    // cpp::SimpleAny storage is of 64-8 bytes, to fit
    // n a cache line (First 8 bytes are used for
    // semantics)
    EXPECT_THROW((SimpleAny::create<std::array<char, 64>>()), cpp::AssertException);
    EXPECT_THROW((SimpleAny::create<std::array<char, 128>>()), cpp::AssertException);
    EXPECT_THROW((SimpleAny::create<std::array<char, 256>>()), cpp::AssertException);
    EXPECT_THROW((SimpleAny::create<std::array<char, 512>>()), cpp::AssertException);
}


TEST(SimpleAnyTest, constructFromValue_smallTypes_noThrow)
{
    EXPECT_NO_THROW(SimpleAny{42});
    EXPECT_NO_THROW(SimpleAny{std::string{"hello, world!"}});
    EXPECT_NO_THROW((SimpleAny{std::vector<std::string>{{std::string{"hello, "}, std::string{"world!"}}}}));
}

TEST(SimpleAnyTest, get_sameType_noThrow)
{
    EXPECT_NO_THROW(SimpleAny(42).get<int>());
}

TEST(SimpleAnyTest, get_differentType_throws)
{
    EXPECT_THROW(SimpleAny(42).get<std::string>(), cpp::AssertException);
}

TEST(SimpleAnyTest, assignValue_smallDifferentTypes_noThrow)
{
    auto any = SimpleAny::create<int>(42);

    EXPECT_NO_THROW(any = std::string("hello, world!"));
    EXPECT_NO_THROW(any = (std::array<char, 4>()));
    EXPECT_NO_THROW(any = (std::array<char, 8>()));
    EXPECT_NO_THROW(any = (std::array<char, 16>()));
    EXPECT_NO_THROW(any = (std::array<char, 32>()));
}

TEST(SimpleAnyTest, assignValue_sameType_noThrow)
{
    auto any = SimpleAny::create<int>(42);

    EXPECT_NO_THROW(any = 42*42);
}
