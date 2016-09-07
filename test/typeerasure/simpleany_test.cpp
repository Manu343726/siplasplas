#define SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS

#include "mocks/valuesemantics.hpp"
#include <siplasplas/typeerasure/anystorage/fixedsize.hpp>
#include <siplasplas/typeerasure/anystorage/deadpool.hpp>
#include <siplasplas/typeerasure/simpleany.hpp>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include <array>

using namespace ::testing;

using SimpleAny = cpp::SimpleAny64;

TEST(SimpleAnyTest, create_defaultConstructibleType_noThrow)
{
    EXPECT_NO_THROW(SimpleAny::create<DefaultConstructible>());
}

TEST(SimpleAnyTest, create_noDefaultConstructibleType_throws)
{
    EXPECT_THROW(SimpleAny::create<NoDefaultConstructible>(), std::runtime_error);
}

TEST(SimpleAnyTest, defaultConstructed_isEmpty)
{
    SimpleAny any;
    EXPECT_TRUE(any.empty());
}

TEST(SimpleAnyTest, constructedWithValue_noEmpty)
{
    EXPECT_FALSE(SimpleAny::create<std::string>().empty());
    EXPECT_FALSE(SimpleAny::create<std::vector<std::string>>().empty());
    EXPECT_FALSE((SimpleAny::create<std::array<char, 4>>()).empty());
    EXPECT_FALSE((SimpleAny::create<std::array<char, 8>>()).empty());
    EXPECT_FALSE((SimpleAny::create<std::array<char, 16>>()).empty());
    EXPECT_FALSE((SimpleAny::create<std::array<char, 32>>()).empty());
    EXPECT_FALSE((SimpleAny::create<std::array<char, 64>>()).empty());
}

TEST(SimpleAnyTest, create_smallTypes_noThrow)
{
    EXPECT_NO_THROW(SimpleAny::create<std::string>());
    EXPECT_NO_THROW(SimpleAny::create<std::vector<std::string>>());
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 4>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 8>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 16>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 32>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 64>>()));
}

TEST(SimpleAnyTest, createWithCustomArgs_smallTypes_noThrow)
{
    EXPECT_NO_THROW(SimpleAny::create<std::string>("hello, world!"));
    EXPECT_NO_THROW(SimpleAny::create<std::vector<std::string>>(42, "hello, world!"));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 4>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 8>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 16>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 32>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 64>>()));
}

TEST(SimpleAnyTest, createWithCustomArgs_bigTypes_noThrow)
{
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 128>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 256>>()));
    EXPECT_NO_THROW((SimpleAny::create<std::array<char, 512>>()));
}

TEST(SimpleAnyTest, constructFromValue_smallTypes_noThrow)
{
    EXPECT_NO_THROW(SimpleAny{42});
    EXPECT_NO_THROW(SimpleAny{std::string{"hello, world!"}});
    EXPECT_NO_THROW((SimpleAny{std::vector<std::string>{}}));
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

TEST(SimpleAnyTest, deadPool8_assignValue_bigDifferentTypes_noThrow)
{
    auto any = cpp::SimpleAny<cpp::DeadPoolStorage<8>>::create<int>(42);

    EXPECT_NO_THROW(any = std::string("hello, world!"));
    EXPECT_NO_THROW(any = (std::array<char, 128>()));
    EXPECT_NO_THROW(any = (std::array<char, 256>()));
    EXPECT_NO_THROW(any = (std::array<char, 512>()));
    EXPECT_NO_THROW(any = (std::array<char, 1024>()));
}

TEST(SimpleAnyTest, assignValue_sameType_noThrow)
{
    auto any = SimpleAny::create<int>(42);

    EXPECT_NO_THROW(any = 42*42);
}
