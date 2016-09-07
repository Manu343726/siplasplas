#include "mocks/invoke.hpp"
#include <siplasplas/typeerasure/field.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp;
using namespace ::cpp::typeerasure;
using namespace ::std::string_literals;

TEST(FieldTest, referencesRightMemberObject)
{
    Field8 stringField8{&Class::str};
    Field8 intField8{&Class::i};
    Field16 stringField16{&Class::str};
    Field16 intField16{&Class::i};
    Field32 stringField32{&Class::str};
    Field32 intField32{&Class::i};
    Field64 stringField64{&Class::str};
    Field64 intField64{&Class::i};

    Class object;

    EXPECT_EQ(TypeInfo::get<std::string>(), stringField8.get(object).typeInfo());
    EXPECT_EQ(TypeInfo::get<std::string>(), stringField16.get(object).typeInfo());
    EXPECT_EQ(TypeInfo::get<std::string>(), stringField32.get(object).typeInfo());
    EXPECT_EQ(TypeInfo::get<std::string>(), stringField64.get(object).typeInfo());

    EXPECT_EQ(TypeInfo::get<int>(), intField8.get(object).typeInfo());
    EXPECT_EQ(TypeInfo::get<int>(), intField16.get(object).typeInfo());
    EXPECT_EQ(TypeInfo::get<int>(), intField32.get(object).typeInfo());
    EXPECT_EQ(TypeInfo::get<int>(), intField64.get(object).typeInfo());


    EXPECT_EQ(&object.str, &stringField8.getAs<std::string>(object));
    EXPECT_EQ(&object.str, &stringField16.getAs<std::string>(object));
    EXPECT_EQ(&object.str, &stringField32.getAs<std::string>(object));
    EXPECT_EQ(&object.str, &stringField64.getAs<std::string>(object));

    EXPECT_EQ(&object.i, &intField8.getAs<int>(object));
    EXPECT_EQ(&object.i, &intField16.getAs<int>(object));
    EXPECT_EQ(&object.i, &intField32.getAs<int>(object));
    EXPECT_EQ(&object.i, &intField64.getAs<int>(object));
}

TEST(FieldTest, AssignValue)
{
    Field64 field{&Class::str};
    Class object;

    field.getAs<std::string>(object) = "hello, world!"s;

    EXPECT_EQ(object.str, "hello, world!");
}
