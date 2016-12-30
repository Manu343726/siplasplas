#include <siplasplas/reflection/parser/api/core/clang/handle.hpp>
#include <gmock/gmock.h>

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::testing;

TEST(NullHandleTest, ptrRawHandle_returnsNullptr)
{
    EXPECT_EQ(nullptr, nullHandle<int*>());
}
