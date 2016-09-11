#include <siplasplas/typeerasure/typeinfo.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp::typeerasure;

TEST(TypeInfoTest, equalityOperator_sameTypesEqual)
{
    EXPECT_EQ(TypeInfo::get<int>(), TypeInfo::get<int>());
    EXPECT_EQ(TypeInfo::get<float>(), TypeInfo::get<float>());
    EXPECT_EQ(TypeInfo::get<long int>(), TypeInfo::get<long int>());
    EXPECT_EQ(TypeInfo::get<std::string>(), TypeInfo::get<std::string>());
}

TEST(TypeInfoTest, equalityOperator_differentTypes_notEqual)
{
    struct MyStruct {};

    EXPECT_NE(TypeInfo::get<int>(), TypeInfo::get<MyStruct>());
    EXPECT_NE(TypeInfo::get<float>(), TypeInfo::get<MyStruct>());
    EXPECT_NE(TypeInfo::get<long int>(), TypeInfo::get<MyStruct>());
    EXPECT_NE(TypeInfo::get<std::string>(), TypeInfo::get<MyStruct>());
}
