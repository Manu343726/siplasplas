#include "mocks/invoke.hpp"
#include <siplasplas/typeerasure/anyarg.hpp>
#include <gmock/gmock.h>

using namespace ::testing;

TEST(AnyArgTest, lvalueReference_modifiesReferencedObject)
{
    std::string str;
    Class object;
    cpp::AnyArg arg(str), arg2(object);

    arg.get<std::string>() = "hello, world!";
    arg2.get<Class>().str = "hello, world!";
    arg2.get<Class>().i   = 42;

    EXPECT_EQ("hello, world!", str);
    EXPECT_EQ("hello, world!", object.str);
    EXPECT_EQ(42, object.i);
}

TEST(AnyArgTest, rightTypeCategories)
{
    auto getLvalueReference = [](std::string& stringRef)
    {
        cpp::AnyArg arg{stringRef};

        EXPECT_TRUE(arg.isLvalue());
        EXPECT_FALSE(arg.isConst());
        EXPECT_FALSE(arg.isRvalue());
    };
    auto getConstLvalueReference = [](const std::string& stringRef)
    {
        cpp::AnyArg arg{stringRef};

        EXPECT_TRUE(arg.isLvalue());
        EXPECT_TRUE(arg.isConst());
        EXPECT_FALSE(arg.isRvalue());
    };
    auto getRvalueReference = [](std::string&& stringRef)
    {
        cpp::AnyArg arg{std::move(stringRef)};

        EXPECT_FALSE(arg.isLvalue());
        EXPECT_FALSE(arg.isConst());
        EXPECT_TRUE(arg.isRvalue());
    };

    std::string str{"hello, world!"};

    getLvalueReference(str);
    getConstLvalueReference(str);
    getRvalueReference(std::move(str));
}
