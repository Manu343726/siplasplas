#include <gmock/gmock.h>
#include "../mocks/iostream.hpp"
#include <siplasplas/typeerasure/concepts/iostream.hpp>

using namespace ::testing;
using namespace ::cpp::concepts;

TEST(ConceptsIOStreamTest, typeWithNoStreamLeftShiftOverload_isNotOStreamable)
{
    EXPECT_FALSE(OStreamable<NoLeftShiftOverload>());
    EXPECT_FALSE(OStreamable<LeftShiftOverload>());
}

TEST(ConceptsIOStreamTest, typeWithStreamLeftShiftOverload_isOStreamable)
{
    EXPECT_TRUE(OStreamable<StreamLeftShiftOverload>());
}

TEST(ConceptsIOStreamTest, standardTypesAreOStreamable)
{
    EXPECT_TRUE(OStreamable<unsigned long long int>());
    EXPECT_TRUE(OStreamable<unsigned long int>());
    EXPECT_TRUE(OStreamable<unsigned int>());
    EXPECT_TRUE(OStreamable<int>());
    EXPECT_TRUE(OStreamable<long int>());
    EXPECT_TRUE(OStreamable<long long int>());
    EXPECT_TRUE(OStreamable<char>());
    EXPECT_TRUE(OStreamable<bool>());
    EXPECT_TRUE(OStreamable<float>());
    EXPECT_TRUE(OStreamable<double>());
    EXPECT_TRUE(OStreamable<decltype("hello")>());
    EXPECT_TRUE(OStreamable<std::string>());
}

TEST(ConceptsIOStreamTest, typeWithNoStreamRightShiftOverload_isNotIStreamable)
{
    EXPECT_FALSE(IStreamable<NoRightShiftOverload>());
    EXPECT_FALSE(IStreamable<RightShiftOverload>());
}

TEST(ConceptsIOStreamTest, typeWithStreamRightShiftOverload_isIStreamable)
{
    EXPECT_TRUE(IStreamable<StreamRightShiftOverload>());
}

TEST(ConceptsIOStreamTest, standardTypesAreIStreamable)
{
    EXPECT_TRUE(IStreamable<unsigned long long int>());
    EXPECT_TRUE(IStreamable<unsigned long int>());
    EXPECT_TRUE(IStreamable<unsigned int>());
    EXPECT_TRUE(IStreamable<int>());
    EXPECT_TRUE(IStreamable<long int>());
    EXPECT_TRUE(IStreamable<long long int>());
    EXPECT_TRUE(IStreamable<char>());
    EXPECT_TRUE(IStreamable<bool>());
    EXPECT_TRUE(IStreamable<float>());
    EXPECT_TRUE(IStreamable<double>());
    EXPECT_TRUE(IStreamable<std::string>());
}
