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
