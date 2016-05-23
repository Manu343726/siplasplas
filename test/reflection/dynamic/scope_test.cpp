#include <gmock/gmock.h>
#include <siplasplas/reflection/dynamic/scope.hpp>

using namespace ::testing;
using namespace cpp::dynamic_reflection;

TEST(ScopeTest, globalScope_fullNameIsEmpty)
{
    EXPECT_EQ("", Scope::globalScope().fullName());
}

TEST(ScopeTest, globalScope_depthIsZero)
{
    EXPECT_EQ(0, Scope::globalScope().depth());
}

TEST(ScopeTest, globalScope_isGlobalScopeReturnsTrue)
{
    EXPECT_TRUE(Scope::globalScope().isGlobalScope());
}

TEST(ScopeTest, globalScope_parentThrows)
{
    EXPECT_THROW(Scope::globalScope().parent(), std::runtime_error);
}

TEST(ScopeTest, firstLevelScope_isNotGlobalScope)
{
    EXPECT_FALSE(Scope::fromFullName("::std").isGlobalScope());
}

TEST(ScopeTest, firstLevelScope_depthIsOne)
{
    EXPECT_EQ(1, Scope::fromFullName("::std").depth());
}

TEST(ScopeTest, firstLevelScope_correctName)
{
    EXPECT_EQ("std", Scope::fromFullName("::std").name());
}

TEST(ScopeTest, firstLevelScope_parentNoThrows)
{
    auto std = Scope::fromFullName("::std");
    EXPECT_NO_THROW(std.parent());
}

TEST(ScopeTest, firstLevelScope_parentIsGlobalScope)
{
    EXPECT_EQ(Scope::globalScope(), Scope::fromFullName("::std").parent());
}

TEST(ScopeTest, firstLevelScope_fromParentScope_givesCorrectScope)
{
    auto parent = Scope::globalScope();
    auto std = Scope::fromParentScope(parent, "std");

    EXPECT_EQ(parent, std.parent());
    EXPECT_EQ("std", std.name());
    EXPECT_EQ("::std", std.fullName());
}
