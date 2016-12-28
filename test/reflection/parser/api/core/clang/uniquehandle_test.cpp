#include "handletest.hpp"
#include <gmock/gmock.h>

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::test;
using namespace ::testing;


class UniqueHandleTest : public HandleTest<UniqueHandleTest>
{
protected:
    using CustomUniqueHandle = UniqueHandle<
        CustomRawHandleType,
        mocks::releaseHandle<CustomRawHandleType, UniqueHandleTest>
    >;
};

TEST_F(UniqueHandleTest, nullHandle_isNull)
{
    CustomUniqueHandle handle{CustomRawHandleType{42}};

    EXPECT_EQ(nullHandle<CustomRawHandleType>(), handle.rawHandle());
    EXPECT_TRUE(handle.isNull());
    EXPECT_TRUE(CustomUniqueHandle::null().isNull());
}

TEST_F(UniqueHandleTest, reset_ReleasesOldHandleIfNotNullOnly)
{
    CustomUniqueHandle notNull{43};
    CustomUniqueHandle null = CustomUniqueHandle::null();

    expectRelease(43).Times(1);
    notNull.reset(25);

    expectRelease(nullHandle<CustomRawHandleType>()).Times(0);
    null.reset(45);

    // Last handles released on destruction:
    expectRelease(45).Times(1);
    expectRelease(25).Times(1);
}

TEST_F(UniqueHandleTest, reset_OwnsNewHandle)
{
    CustomUniqueHandle handle{43};

    expectRelease(43);
    handle.reset(44);

    // The new handle is released on destruction:
    expectRelease(44);
}

TEST_F(UniqueHandleTest, reset_noArgs_oldHandleReleasedAndBecomesNull)
{
    CustomUniqueHandle handle{43};

    expectRelease(43);
    handle.reset();
    EXPECT_TRUE(handle.isNull());
}

TEST_F(UniqueHandleTest, release_oldRawHandleIsNotReleased)
{
    expectRelease(43).Times(0);
    CustomUniqueHandle notNull{43};

    auto rawHandle = notNull.release();
    EXPECT_EQ(43, rawHandle.value);
}

TEST_F(UniqueHandleTest, moveCtor_takesOwnershipOfHandle)
{
    CustomUniqueHandle rhs{43};
    CustomUniqueHandle lhs{std::move(rhs)};

    EXPECT_TRUE(rhs.isNull());
    EXPECT_FALSE(lhs.isNull());

    expectRelease(43);
}

TEST_F(UniqueHandleTest, moveAssign_takesOwnershipOfHandle)
{
    CustomUniqueHandle rhs{43};
    CustomUniqueHandle lhs = CustomUniqueHandle::null();

    lhs = std::move(rhs);

    EXPECT_TRUE(rhs.isNull());
    EXPECT_FALSE(lhs.isNull());

    expectRelease(43);
}

TEST_F(UniqueHandleTest, handleAssignment_ReleasesOldHandleAndTakesOwnershipOfNewHandle)
{
    CustomUniqueHandle handle{43};

    expectRelease(43);
    handle = 44;

    EXPECT_FALSE(handle.isNull());
    expectRelease(44);
}
