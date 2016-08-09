#include <gmock/gmock.h>
#include "../mocks/iostream.hpp"
#include <siplasplas/typeerasure/features/iostream.hpp>
#include <iostream>

using namespace ::testing;
using namespace ::cpp::features;

TEST(FeaturesIOStreamTest, OStreamableFeature_OStreamableType_noThrow)
{
    EXPECT_NO_THROW(OStreamable::apply(std::cout, StreamLeftShiftOverload()));
}

TEST(FeaturesIOStreamTest, OStreamableFeature_noOStreamableType_throws)
{
    EXPECT_THROW(OStreamable::apply(std::cout, NoLeftShiftOverload()), std::runtime_error);
}

TEST(FeaturesIOStreamTest, IStreamableFeature_IStreamableType_noThrow)
{
    StreamRightShiftOverload lvalue;
    EXPECT_NO_THROW(IStreamable::apply(std::cin, lvalue));
}

TEST(FeaturesIOStreamTest, IStreamableFeature_noIStreamableType_throws)
{
    NoRightShiftOverload lvalue;
    EXPECT_THROW(IStreamable::apply(std::cin, lvalue), std::runtime_error);
}
