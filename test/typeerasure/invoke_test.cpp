#define SIPLASPLAS_TYPEERASURE_SIMPLEANY_TYPECHECKS

#include "mocks/invoke.hpp"
#include <siplasplas/typeerasure/invoke.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::std::string_literals;


TEST(InvokeTest, SimpleAny64_FreeFunctionIntsByValue_rightArgs)
{
    EXPECT_EQ(42, cpp::typeerasure::invoke(addIntsByValue, cpp::SimpleAny64(20), cpp::SimpleAny64(22)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(addIntsByValue, std::vector<cpp::SimpleAny64>{20, 22}));
}

TEST(InvokeTest, SimpleAny64_FreeFunctionStringsByConstReference_rightArgs)
{
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(addStringsByConstReference, cpp::SimpleAny64("hello, "s), cpp::SimpleAny64("world!"s)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(addStringsByConstReference, std::vector<cpp::SimpleAny64>{"hello, "s, "world!"s}));
}

TEST(InvokeTest, SimpleAny64_FreeFunctionIntsByValue_wrongArgsAssertThrows)
{
    EXPECT_THROW(cpp::typeerasure::invoke(addIntsByValue, cpp::SimpleAny64("hello, "s), cpp::SimpleAny64("world!"s)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(addIntsByValue, std::vector<cpp::SimpleAny64>{"hello, "s, "world!"s}), cpp::AssertException);
}

TEST(InvokeTest, SimpleAny64_FreeFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    EXPECT_THROW(cpp::typeerasure::invoke(addStringsByConstReference, cpp::SimpleAny64(42), cpp::SimpleAny64(42)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(addStringsByConstReference, std::vector<cpp::SimpleAny64>{42, 42}), cpp::AssertException);
}

TEST(InvokeTest, SimpleAny64_MemberFunctionIntsByValue_rightArgs)
{
    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValue, cpp::SimpleAny64(Class()), cpp::SimpleAny64(20), cpp::SimpleAny64(22)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValue, std::vector<cpp::SimpleAny64>{Class(), 20, 22}));
}

TEST(InvokeTest, SimpleAny64_MemberFunctionStringsByConstReference_rightArgs)
{
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReference, cpp::SimpleAny64(Class()), cpp::SimpleAny64("hello, "s), cpp::SimpleAny64("world!"s)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReference, std::vector<cpp::SimpleAny64>{Class(), "hello, "s, "world!"s}));
}

TEST(InvokeTest, SimpleAny64_MemberFunctionIntsByValue_wrongArgsAssertThrows)
{
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValue, cpp::SimpleAny64(Class()), cpp::SimpleAny64("hello, "s), cpp::SimpleAny64("world!"s)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValue, std::vector<cpp::SimpleAny64>{Class(), "hello, "s, "world!"s}), cpp::AssertException);
}

TEST(InvokeTest, SimpleAny64_MemberFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReference, cpp::SimpleAny64(Class()), cpp::SimpleAny64(42), cpp::SimpleAny64(42)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReference, std::vector<cpp::SimpleAny64>{Class(), 42, 42}), cpp::AssertException);
}

TEST(InvokeTest, SimpleAny64_ConstMemberFunctionIntsByValue_rightArgs)
{
    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::SimpleAny64(Class()), cpp::SimpleAny64(20), cpp::SimpleAny64(22)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::SimpleAny64>{Class(), 20, 22}));
}

TEST(InvokeTest, SimpleAny64_ConstMemberFunctionStringsByConstReference_rightArgs)
{
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::SimpleAny64(Class()), cpp::SimpleAny64("hello, "s), cpp::SimpleAny64("world!"s)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::SimpleAny64>{Class(), "hello, "s, "world!"s}));
}

TEST(InvokeTest, SimpleAny64_ConstMemberFunctionIntsByValue_wrongArgsAssertThrows)
{
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::SimpleAny64(Class()), cpp::SimpleAny64("hello, "s), cpp::SimpleAny64("world!"s)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::SimpleAny64>{Class(), "hello, "s, "world!"s}), cpp::AssertException);
}

TEST(InvokeTest, SimpleAny64_ConstMemberFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::SimpleAny64(Class()), cpp::SimpleAny64(42), cpp::SimpleAny64(42)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::SimpleAny64>{Class(), 42, 42}), cpp::AssertException);
}

TEST(InvokeTest, SimpleAny64_byRefParamFunction_modifiesAnyHostedObject)
{
    auto any = cpp::SimpleAny64::create<std::string>("hello, ");
    auto mutation = [](std::string& stringRef)
    {
        stringRef += "world!";
    };

    cpp::typeerasure::invoke(mutation, any);

    EXPECT_EQ("hello, world!", any.get<std::string>());

    class MutableClass
    {
    public:
        void mutableFunction()
        {
            i = 42;
        }

        int i = 0;
    };

    MutableClass object;

    cpp::typeerasure::invoke(&MutableClass::mutableFunction, cpp::SimpleAny64(&object));
    EXPECT_EQ(42, object.i);
}

TEST(InvokeTest, ConstReferenceSimpleAny_FreeFunctionIntsByValue_rightArgs)
{
    int a = 20;
    int b = 22;

    EXPECT_EQ(42, cpp::typeerasure::invoke(addIntsByValue, cpp::ConstReferenceSimpleAny(a), cpp::ConstReferenceSimpleAny(b)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(addIntsByValue, std::vector<cpp::ConstReferenceSimpleAny>{a, b}));
}

TEST(InvokeTest, ConstReferenceSimpleAny_FreeFunctionStringsByConstReference_rightArgs)
{
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(addStringsByConstReference, cpp::ConstReferenceSimpleAny(hello), cpp::ConstReferenceSimpleAny(world)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(addStringsByConstReference, std::vector<cpp::ConstReferenceSimpleAny>{hello, world}));
}

TEST(InvokeTest, ConstReferenceSimpleAny_FreeFunctionIntsByValue_wrongArgsAssertThrows)
{
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_THROW(cpp::typeerasure::invoke(addIntsByValue, cpp::ConstReferenceSimpleAny(hello), cpp::ConstReferenceSimpleAny(world)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(addIntsByValue, std::vector<cpp::ConstReferenceSimpleAny>{hello, world}), cpp::AssertException);
}

TEST(InvokeTest, ConstReferenceSimpleAny_FreeFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    int a = 20;
    int b = 22;

    EXPECT_THROW(cpp::typeerasure::invoke(addStringsByConstReference, cpp::ConstReferenceSimpleAny(a), cpp::ConstReferenceSimpleAny(b)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(addStringsByConstReference, std::vector<cpp::ConstReferenceSimpleAny>{a, b}), cpp::AssertException);
}

TEST(InvokeTest, ConstReferenceSimpleAny_ConstMemberFunctionIntsByValue_rightArgs)
{
    Class object;
    int a = 20;
    int b = 22;

    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::ConstReferenceSimpleAny(object), cpp::ConstReferenceSimpleAny(a), cpp::ConstReferenceSimpleAny(b)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::ConstReferenceSimpleAny>{object, a, b}));
}

TEST(InvokeTest, ConstReferenceSimpleAny_ConstMemberFunctionStringsByConstReference_rightArgs)
{
    Class object;
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::ConstReferenceSimpleAny(object), cpp::ConstReferenceSimpleAny(hello), cpp::ConstReferenceSimpleAny(world)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::ConstReferenceSimpleAny>{object, hello, world}));
}

TEST(InvokeTest, ConstReferenceSimpleAny_ConstMemberFunctionIntsByValue_wrongArgsAssertThrows)
{
    Class object;
    std::string hello = "hello, ";
    std::string world = "world!";
    int a = 20;
    int b = 22;

    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::ConstReferenceSimpleAny(object), cpp::ConstReferenceSimpleAny(hello), cpp::ConstReferenceSimpleAny(world)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::ConstReferenceSimpleAny>{object, hello, world}), cpp::AssertException);
    /* wrong object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::ConstReferenceSimpleAny(hello), cpp::ConstReferenceSimpleAny(a), cpp::ConstReferenceSimpleAny(b)), cpp::AssertException);
    /* worng object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::ConstReferenceSimpleAny>{hello, a, b}), cpp::AssertException);
}

TEST(InvokeTest, ConstReferenceSimpleAny_ConstMemberFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    Class object;
    int a = 20;
    int b = 22;
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::ConstReferenceSimpleAny(object), cpp::ConstReferenceSimpleAny(a), cpp::ConstReferenceSimpleAny(b)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::ConstReferenceSimpleAny>{object, a, b}), cpp::AssertException);
    /* wrong object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::ConstReferenceSimpleAny(hello), cpp::ConstReferenceSimpleAny(hello), cpp::ConstReferenceSimpleAny(world)), cpp::AssertException);
    /* worng object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::ConstReferenceSimpleAny>{hello, hello, world}), cpp::AssertException);
}

TEST(InvokeTest, ReferenceSimpleAny_FreeFunctionIntsByValue_rightArgs)
{
    int a = 20;
    int b = 22;

    EXPECT_EQ(42, cpp::typeerasure::invoke(addIntsByValue, cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(addIntsByValue, std::vector<cpp::ReferenceSimpleAny>{a, b}));
}

TEST(InvokeTest, ReferenceSimpleAny_FreeFunctionStringsByConstReference_rightArgs)
{
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(addStringsByConstReference, cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(addStringsByConstReference, std::vector<cpp::ReferenceSimpleAny>{hello, world}));
}

TEST(InvokeTest, ReferenceSimpleAny_FreeFunctionIntsByValue_wrongArgsAssertThrows)
{
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_THROW(cpp::typeerasure::invoke(addIntsByValue, cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(addIntsByValue, std::vector<cpp::ReferenceSimpleAny>{hello, world}), cpp::AssertException);
}

TEST(InvokeTest, ReferenceSimpleAny_FreeFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    int a = 20;
    int b = 22;

    EXPECT_THROW(cpp::typeerasure::invoke(addStringsByConstReference, cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(addStringsByConstReference, std::vector<cpp::ReferenceSimpleAny>{a, b}), cpp::AssertException);
}

TEST(InvokeTest, ReferenceSimpleAny_MemberFunctionIntsByValue_rightArgs)
{
    Class object;
    int a = 20;
    int b = 22;

    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValue, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValue, std::vector<cpp::ReferenceSimpleAny>{object, a, b}));
}

TEST(InvokeTest, ReferenceSimpleAny_MemberFunctionStringsByConstReference_rightArgs)
{
    Class object;
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReference, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReference, std::vector<cpp::ReferenceSimpleAny>{object, hello, world}));
}

TEST(InvokeTest, ReferenceSimpleAny_MemberFunctionIntsByValue_wrongArgsAssertThrows)
{
    Class object;
    std::string hello = "hello, ";
    std::string world = "world!";
    int a = 20;
    int b = 22;

    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValue, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValue, std::vector<cpp::ReferenceSimpleAny>{object, hello, world}), cpp::AssertException);
    /* wrong object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValue, cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)), cpp::AssertException);
    /* worng object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValue, std::vector<cpp::ReferenceSimpleAny>{hello, a, b}), cpp::AssertException);
}

TEST(InvokeTest, ReferenceSimpleAny_MemberFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    Class object;
    int a = 20;
    int b = 22;
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReference, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReference, std::vector<cpp::ReferenceSimpleAny>{object, a, b}), cpp::AssertException);
    /* wrong object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReference, cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)), cpp::AssertException);
    /* worng object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReference, std::vector<cpp::ReferenceSimpleAny>{hello, hello, world}), cpp::AssertException);
}

TEST(InvokeTest, ReferenceSimpleAny_ConstMemberFunctionIntsByValue_rightArgs)
{
    Class object;
    int a = 20;
    int b = 22;

    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)));
    EXPECT_EQ(42, cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::ReferenceSimpleAny>{object, a, b}));
}

TEST(InvokeTest, ReferenceSimpleAny_ConstMemberFunctionStringsByConstReference_rightArgs)
{
    Class object;
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)));
    EXPECT_EQ("hello, world!", cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::ReferenceSimpleAny>{object, hello, world}));
}

TEST(InvokeTest, ReferenceSimpleAny_ConstMemberFunctionIntsByValue_wrongArgsAssertThrows)
{
    Class object;
    std::string hello = "hello, ";
    std::string world = "world!";
    int a = 20;
    int b = 22;

    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::ReferenceSimpleAny>{object, hello, world}), cpp::AssertException);
    /* wrong object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)), cpp::AssertException);
    /* worng object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addIntsByValueConst, std::vector<cpp::ReferenceSimpleAny>{hello, a, b}), cpp::AssertException);
}

TEST(InvokeTest, ReferenceSimpleAny_ConstMemberFunctionStringsByConstReference_wrongArgsAssertThrows)
{
    Class object;
    int a = 20;
    int b = 22;
    std::string hello = "hello, ";
    std::string world = "world!";

    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::ReferenceSimpleAny(object), cpp::ReferenceSimpleAny(a), cpp::ReferenceSimpleAny(b)), cpp::AssertException);
    EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::ReferenceSimpleAny>{object, a, b}), cpp::AssertException);
    /* wrong object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(hello), cpp::ReferenceSimpleAny(world)), cpp::AssertException);
    /* worng object type */ EXPECT_THROW(cpp::typeerasure::invoke(&Class::addStringsByConstReferenceConst, std::vector<cpp::ReferenceSimpleAny>{hello, hello, world}), cpp::AssertException);
}

TEST(InvokeTest, ReferenceSimpleAny_byRefParamFunction_modifiesAnyHostedObject)
{
    std::string string{"hello, "};
    cpp::ReferenceSimpleAny any{string};

    auto mutation = [](std::string& stringRef)
    {
        stringRef += "world!";
    };

    cpp::typeerasure::invoke(mutation, any);

    EXPECT_EQ("hello, world!", string);
}

TEST(InvokeTest, AnyArg_memberFunctionModifiesObject)
{
    class Class
    {
    public:
        void f()
        {
            i = 42;
        }

        int i = 0;
    };

    Class object;

    cpp::typeerasure::invoke(&Class::f, std::vector<cpp::AnyArg>{object});

    EXPECT_EQ(42, object.i);
}
