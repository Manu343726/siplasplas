#include "handletest.hpp"
#include <gmock/gmock.h>

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::test;
using namespace ::testing;

class SharedHandleTest : public HandleTest<SharedHandleTest>
{
protected:
    using CustomSharedHandle = SharedHandle<
        CustomRawHandleType,
        mocks::releaseHandle<CustomRawHandleType, SharedHandleTest>
    >;
};

TEST_F(SharedHandleTest, handleReleasedByLastSharedHandleOnly)
{
    expectRelease(43);
    CustomSharedHandle handle{43};
    CustomSharedHandle handle2 = handle;
    CustomSharedHandle handle3 = std::move(handle);
}

TEST_F(SharedHandleTest, allCopiedSharedHandlesShareSameProperties)
{
    CustomSharedHandle handle{43};

    CustomSharedHandle handle2{handle};
    CustomSharedHandle handle3{handle2};

    EXPECT_FALSE(handle.isNull());
    EXPECT_FALSE(handle2.isNull());
    EXPECT_FALSE(handle3.isNull());

    handle.release();

    EXPECT_TRUE(handle.isNull());
    EXPECT_TRUE(handle2.isNull());
    EXPECT_TRUE(handle3.isNull());
}
