#include "mocks/valuesemantics.hpp"
#include <siplasplas/typeerasure/simpleany.hpp>
#include <gmock/gmock.h>

using namespace ::cpp;
using namespace ::testing;

TEST(ConstReferenceAnyTest, referencesSameObject)
{
    int i = 42;
    auto any = ConstReferenceAny::create(i);

    EXPECT_EQ(&i, &any.get<int>());
    EXPECT_EQ(any.get<int>(), i);
    i = 42*42;
    EXPECT_EQ(any.get<int>(), i);
}

TEST(ConstReferenceAnyTest, get_rightType_noThrow)
{
    int i = 42;
    EXPECT_NO_THROW(ConstReferenceAny::create(i).get<decltype(i)>());
}

TEST(ConstReferenceAnyTest, get_wrongType_throws)
{
    int i = 42;
    EXPECT_THROW(ConstReferenceAny::create(i).get<bool>(), AssertException);
}

TEST(ReferenceAnyTest, referencesSameObject)
{
    int i = 42;
    auto any = ReferenceAny::create(i);

    EXPECT_EQ(&i, &any.get<int>());
    EXPECT_EQ(any.get<int>(), i);
    i = 42*42;
    EXPECT_EQ(any.get<int>(), i);
    any.get<int>() = 42;
    EXPECT_EQ(any.get<int>(), i);
}

TEST(ReferenceAnyTest, get_rightType_noThrow)
{
    int i = 42;
    EXPECT_NO_THROW(ReferenceAny::create(i).get<decltype(i)>());
}

TEST(ReferenceAnyTest, get_wrongType_throws)
{
    int i = 42;
    EXPECT_THROW(ReferenceAny::create(i).get<bool>(), AssertException);
}
