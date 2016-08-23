#include <siplasplas/typeerasure/anyarg.hpp>
#include <gmock/gmock.h>

using namespace ::testing;

class Class
{
public:
    int i = 0;
};

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

TEST(AnyArgTest, lvalueReference_modifiesReferencedObject)
{
    Class object;
    cpp::AnyArg arg(object);

    arg.get<Class>().i = 42;

    EXPECT_EQ(42, object.i);
}
