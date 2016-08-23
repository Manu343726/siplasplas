#include <gmock/gmock.h>
#include "../mocks/iostream.hpp"
#include <siplasplas/typeerasure/features/iostream.hpp>
#include <sstream>

using namespace ::testing;
using namespace ::cpp::features;

TEST(FeaturesIOStreamTest, OStreamableFeature_OStreamableType_noThrow)
{
    std::ostringstream os;
    EXPECT_NO_THROW(OStreamable::apply(os, StreamLeftShiftOverload()));
}

TEST(FeaturesIOStreamTest, OStreamableFeature_noOStreamableType_throws)
{
    std::ostringstream os;
    EXPECT_THROW(OStreamable::apply(os, NoLeftShiftOverload()), std::runtime_error);
}

TEST(FeaturesIOStreamTest, IStreamableFeature_IStreamableType_noThrow)
{
    std::istringstream is;
    StreamRightShiftOverload lvalue;
    EXPECT_NO_THROW(IStreamable::apply(is, lvalue));
}

TEST(FeaturesIOStreamTest, IStreamableFeature_noIStreamableType_throws)
{
    std::istringstream is;
    NoRightShiftOverload lvalue;
    EXPECT_THROW(IStreamable::apply(is, lvalue), std::runtime_error);
}
