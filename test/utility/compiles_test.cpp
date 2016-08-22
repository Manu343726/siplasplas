#include <siplasplas/utility/compiles.hpp>
#include <gmock/gmock.h>

using namespace ::testing;

TEST(CompilesTest, works)
{
    std::string str;
    EXPECT_FALSE(SIPLASPLAS_UTILITY_COMPILES(identity(str)++));
    auto compiles = SIPLASPLAS_UTILITY_COMPILES(identity(str)++);
    static_assert(!compiles, "CompilesTest_works test failed");
}
