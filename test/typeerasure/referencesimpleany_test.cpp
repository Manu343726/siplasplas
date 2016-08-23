#define SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS

#include "mocks/valuesemantics.hpp"
#include <siplasplas/typeerasure/simpleany.hpp>
#include <gmock/gmock.h>

using namespace ::cpp;
using namespace ::testing;

TEST(ConstReferenceSimpleAnyTest, referencesSameObject)
{
    int i = 42;
    auto any = ConstReferenceSimpleAny::create(i);

    EXPECT_EQ(&i, &any.get<int>());
    EXPECT_EQ(any.get<int>(), i);
    i = 42*42;
    EXPECT_EQ(any.get<int>(), i);
}

TEST(ConstReferenceSimpleAnyTest, get_rightType_noThrow)
{
    int i = 42;
    EXPECT_NO_THROW(ConstReferenceSimpleAny::create(i).get<decltype(i)>());
}

TEST(ConstReferenceSimpleAnyTest, get_wrongType_throws)
{
    int i = 42;
    EXPECT_THROW(ConstReferenceSimpleAny::create(i).get<bool>(), AssertException);
}

TEST(ReferenceSimpleAnyTest, referencesSameObject)
{
    int i = 42;
    auto any = ReferenceSimpleAny::create(i);

    EXPECT_EQ(&i, &any.get<int>());
    EXPECT_EQ(any.get<int>(), i);
    i = 42*42;
    EXPECT_EQ(any.get<int>(), i);
    any.get<int>() = 42;
    EXPECT_EQ(any.get<int>(), i);
}

TEST(ReferenceSimpleAnyTest, get_rightType_noThrow)
{
    int i = 42;
    EXPECT_NO_THROW(ReferenceSimpleAny::create(i).get<decltype(i)>());
}

TEST(ReferenceSimpleAnyTest, get_wrongType_throws)
{
    int i = 42;
    EXPECT_THROW(ReferenceSimpleAny::create(i).get<bool>(), AssertException);
}
