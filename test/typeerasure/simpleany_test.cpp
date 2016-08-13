#include "mocks/valuesemantics.hpp"
#include <siplasplas/typeerasure/simpleany.hpp>
#include <gmock/gmock.h>

using namespace ::cpp;
using namespace ::testing;

TEST(SimpleAnyTest, create_defaultConstructibleType_noThrow)
{
    EXPECT_NO_THROW(SimpleAny::create<DefaultConstructible>());   
}
