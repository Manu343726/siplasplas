#include <gmock/gmock.h>
#include "../mocks/valuesemantics.hpp"
#include <siplasplas/typeerasure/features/valuesemantics.hpp>
#include <string>

using namespace ::testing;
using namespace ::cpp;

TEST(FeaturesValueSemanticsTest, DefaultConstructibleFeature_defaultConstructibleType_noThrow)
{
    EXPECT_NO_THROW(features::DefaultConstructible::apply<DefaultConstructible>());
}

TEST(FeaturesValueSemanticsTest, DefaultConstructibleFeature_noDefaultConstructibleType_throws)
{
    EXPECT_THROW(features::DefaultConstructible::apply<NoDefaultConstructible>(), std::runtime_error);
}

TEST(FeaturesValueSemanticsTest, ConstructibleFeature_constructibleType_noThrow)
{
    EXPECT_NO_THROW(features::Constructible::apply<Constructible>());
    EXPECT_NO_THROW(features::Constructible::apply<Constructible>(42));
    EXPECT_NO_THROW(features::Constructible::apply<Constructible>(42, true));
    EXPECT_NO_THROW(features::Constructible::apply<Constructible>(42, true, std::string("hello")));
}

TEST(FeaturesValueSemanticsTest, ConstructibleFeature_noConstructibleType_throws)
{
    EXPECT_THROW(features::Constructible::apply<NoConstructible>(), std::runtime_error);
    EXPECT_THROW(features::Constructible::apply<NoConstructible>(42), std::runtime_error);
    EXPECT_THROW(features::Constructible::apply<NoConstructible>(42, true), std::runtime_error);
    EXPECT_THROW(features::Constructible::apply<NoConstructible>(42, true, std::string("hello")), std::runtime_error);
}

TEST(FeaturesValueSemanticsTest, CopyConstructibleFeature_copyConstructibleType_noThrow)
{
    CopyConstructible lvalue;
    EXPECT_NO_THROW(features::CopyConstructible::apply(lvalue));
    EXPECT_NO_THROW(features::CopyConstructible::apply(CopyConstructible()));
}

TEST(FeaturesValueSemanticsTest, CopyConstructibleFeature_noCopyConstructibleType_throws)
{
    NoCopyConstructible lvalue;
    EXPECT_THROW(features::CopyConstructible::apply(lvalue), std::runtime_error);
    EXPECT_THROW(features::CopyConstructible::apply(NoCopyConstructible()), std::runtime_error);
}

TEST(FeaturesValueSemanticsTest, MoveConstructibleFeature_moveConstructibleType_noThrow)
{
    EXPECT_NO_THROW(features::MoveConstructible::apply(MoveConstructible()));
}

TEST(FeaturesValueSemanticsTest, MoveConstructibleFeature_noMoveConstructibleType_throws)
{
    EXPECT_THROW(features::MoveConstructible::apply(NoMoveConstructible()), std::runtime_error);
}

TEST(FeaturesValueSemanticsTest, AssignableFeature_assignableType_noThrow)
{
    Assignable lvalue;
    int intLvalue;
    EXPECT_NO_THROW(features::Assignable::apply(lvalue, intLvalue));
    EXPECT_NO_THROW(features::Assignable::apply(lvalue, 42));
}

TEST(FeaturesValueSemanticsTest, AssignableFeature_noAssignableType_throws)
{
    NoAssignable lvalue;
    int intLvalue;
    EXPECT_THROW(features::Assignable::apply(lvalue, intLvalue), std::runtime_error);
    EXPECT_THROW(features::Assignable::apply(lvalue, 42), std::runtime_error);
}

TEST(FeaturesValueSemanticsTest, CopyAssignableFeature_copyAssignableType_noThrow)
{
    CopyAssignable lvalue;
    EXPECT_NO_THROW(features::CopyAssignable::apply(lvalue, lvalue));
}

TEST(FeaturesValueSemanticsTest, CopyAssignableFeature_noCopyAssignableType_throws)
{
    NoCopyAssignable lvalue;
    EXPECT_THROW(features::Assignable::apply(lvalue, lvalue), std::runtime_error);
}

TEST(FeaturesValueSemanticsTest, MoveAssignableFeature_moveAssignableType_noThrow)
{
    MoveAssignable lvalue;
    EXPECT_NO_THROW(features::MoveAssignable::apply(lvalue, MoveAssignable()));
}

TEST(FeaturesValueSemanticsTest, MoveAssignableFeature_noMoveAssignableType_throws)
{
    NoMoveAssignable lvalue;
    EXPECT_THROW(features::MoveAssignable::apply(lvalue, NoMoveAssignable()), std::runtime_error);
}

TEST(FeaturesValueSemanticsTest, DestructibleFeature_destructibleType_noThrow)
{
    Destructible lvalue;
    EXPECT_NO_THROW(features::Destructible::apply(lvalue));
}

TEST(FeaturesValueSemanticsTest, DestructibleFeature_noDestructibleType_throws)
{
    auto* noDestructiblePtr = static_cast<NoDestructible*>(std::malloc(sizeof(NoDestructible)));
    ASSERT_NE(noDestructiblePtr, nullptr);
    NoDestructible& lvalue = *noDestructiblePtr;

    EXPECT_THROW(features::Destructible::apply(lvalue), std::runtime_error);

    std::free(noDestructiblePtr);
}
