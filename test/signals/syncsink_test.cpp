#include "sinktest.hpp"
#include <siplasplas/signals/syncsink.hpp>


template<typename Test>
class SyncSinkTest : public ::testing::Test
{
};

TYPED_TEST_CASE_P(SyncSinkTest);


TYPED_TEST_P(SyncSinkTest, invoke_callsCalleeSlot)
{
    TypeParam test;

    EXPECT_CALL(test.callee, slotByValue(signals_test::value<TypeParam>()));
    EXPECT_CALL(test.callee, slotByConstReference(signals_test::value<TypeParam>()));

    (*test.sinkByValue)(signals_test::value<TypeParam>());
    (*test.sinkByConstReference)(signals_test::value<TypeParam>());
}

REGISTER_TYPED_TEST_CASE_P(SyncSinkTest, invoke_callsCalleeSlot);
INSTANTIATE_TYPED_TEST_CASE_P(SyncSinkTest_default, SyncSinkTest, ::signals_test::Tests<::cpp::SyncSink>);
