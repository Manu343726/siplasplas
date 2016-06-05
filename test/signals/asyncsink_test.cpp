#include "sinktest.hpp"
#include <siplasplas/signals/asyncsink.hpp>


template<typename Test>
class AsyncSinkTest : public ::testing::Test
{
};

TYPED_TEST_CASE_P(AsyncSinkTest);


TYPED_TEST_P(AsyncSinkTest, invokeDonesntCallsCalleeSlot)
{
    TypeParam test;

    EXPECT_CALL(test.callee, slotByValue(signals_test::value<TypeParam>()))
        .Times(0);
    EXPECT_CALL(test.callee, slotByConstReference(signals_test::value<TypeParam>()))
        .Times(0);

    (*test.sinkByValue)(signals_test::value<TypeParam>());
    (*test.sinkByConstReference)(signals_test::value<TypeParam>());
}

TYPED_TEST_P(AsyncSinkTest, notInvokedSink_pullDoesntCallCalleeSlot)
{
    TypeParam test;

    EXPECT_CALL(test.callee, slotByValue(signals_test::value<TypeParam>()))
        .Times(0);
    EXPECT_CALL(test.callee, slotByConstReference(signals_test::value<TypeParam>()))
        .Times(0);

    EXPECT_FALSE(test.sinkByValue->pull());
    EXPECT_FALSE(test.sinkByConstReference->pull());
}

TYPED_TEST_P(AsyncSinkTest, invokedSink_pullCallsCalleeSlot)
{
    TypeParam test;

    EXPECT_CALL(test.callee, slotByValue(signals_test::value<TypeParam>()));
    EXPECT_CALL(test.callee, slotByConstReference(signals_test::value<TypeParam>()));

    (*test.sinkByValue)(signals_test::value<TypeParam>());
    (*test.sinkByConstReference)(signals_test::value<TypeParam>());

    EXPECT_TRUE(test.sinkByValue->pull());
    EXPECT_TRUE(test.sinkByConstReference->pull());
}



REGISTER_TYPED_TEST_CASE_P(AsyncSinkTest,
    invokeDonesntCallsCalleeSlot,
    notInvokedSink_pullDoesntCallCalleeSlot,
    invokedSink_pullCallsCalleeSlot
);
INSTANTIATE_TYPED_TEST_CASE_P(AsyncSinkTest_default, AsyncSinkTest, ::signals_test::Tests<::cpp::AsyncSink>);
