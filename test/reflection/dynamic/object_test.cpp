#include <gmock/gmock.h>
#include <siplasplas/reflection/dynamic/object.hpp>

#include "mockspecialfunctions.hpp"

using namespace ::testing;
using namespace ::cpp;
using namespace ::cpp::dynamic_reflection;
using namespace ::std::string_literals;


class ObjectTest : public MockSpecialFunctionsTest, public Test
{
public:
    ObjectTest() :
        MockSpecialFunctionsTest{
            {
                (type1a, nullptr),
                (type1b, nullptr),
                (type2a, nullptr),
                (type2b, nullptr)
            },
            cpp::meta::identity<Type1>(),
            cpp::meta::identity<Type2>()
        },
        emptyObject{},
        nonEmptyObject{42},
        type1a{Type::get<Type1>()},
        type1b{Type::get<Type1>()},
        type2a{Type::get<Type2>()},
        type2b{Type::get<Type2>()}
    {}

    void SetUp()
    {
        ASSERT_TRUE(emptyObject.empty());
        ASSERT_FALSE(nonEmptyObject.empty());
    }

protected:
    Object emptyObject, nonEmptyObject;

    using Type1 = MockSpecialFunctions<struct Type1>;
    using Type2 = MockSpecialFunctions<struct Type2>;

    Object type1a, type1b;
    Object type2a, type2b;
};


TEST_F(ObjectTest, defaultConstructed_empty)
{
    EXPECT_TRUE(emptyObject.empty());
}

TEST_F(ObjectTest, constructFromType_notEmpty)
{
    Object object{Type::get<int>()}; // Default constructs int
    EXPECT_FALSE(object.empty());
}

TEST_F(ObjectTest, constructFromValue_notEmpty)
{
    Object object(1);
    EXPECT_FALSE(object.empty());
}

TEST_F(ObjectTest, copyConstructFromEmptyObject_empty)
{
    Object other{emptyObject};
    EXPECT_TRUE(other.empty());
}

TEST_F(ObjectTest, moveConstructFromEmptyObject_empty)
{
    Object other{std::move(emptyObject)};
    EXPECT_TRUE(other.empty());
}

TEST_F(ObjectTest, copyConstructFromNonEmptyObject_notEmpty)
{
    Object other{nonEmptyObject};
    EXPECT_FALSE(other.empty());
}

TEST_F(ObjectTest, moveConstructFromNonEmptyObject_notEmpty)
{
    Object other{std::move(nonEmptyObject)};
    EXPECT_FALSE(other.empty());
}

TEST_F(ObjectTest, constructFromType_invokesTypeDefaultConstructor)
{
    EXPECT_CALL(functions, default_constructor(_));
    Object objectFromType{Type::get<Type1>()};
}

TEST_F(ObjectTest, constructFromValue_invokesCopyConstructor)
{
    EXPECT_CALL(functions, default_constructor(_)); // Value ctor
    Type1 value;
    EXPECT_CALL(functions, copy_constructor(_, &value));
    Object objectFromValue{value};
}

TEST_F(ObjectTest, constructFromValueAsReference_notInvokeCopyConstructor)
{
    EXPECT_CALL(functions, default_constructor(_)); // Value ctor
    Type1 value;
    EXPECT_CALL(functions, copy_constructor(_, &value)).Times(0);
    Object referenceObject{Type::get<Type1>(), &value, Object::ConstructReference};
    EXPECT_EQ(referenceObject.raw(), &value);
}

TEST_F(ObjectTest, copyConstructFromObject_invokesCopyConstructor)
{
    EXPECT_CALL(functions, copy_constructor(_, type1a.raw()));
    Object objectFromObject{type1a};
}

TEST_F(ObjectTest, copyConstructFromReferenceObject_notInvokeCopyConstructor)
{
    EXPECT_CALL(functions, default_constructor(_)); // Value ctor
    Type1 value;
    Object referenceObject{Type::get<Type1>(), &value, Object::ConstructReference};
    EXPECT_CALL(functions, copy_constructor(_, &value)).Times(0);
    Object copyOfReferenceObject{referenceObject};
    EXPECT_TRUE(copyOfReferenceObject.isReference());
    EXPECT_EQ(copyOfReferenceObject.raw(), &value);
}

TEST_F(ObjectTest, moveConstructFromObject_invokesMoveConstructor)
{
    EXPECT_CALL(functions, move_constructor(_, type1a.raw()));
    Object objectFromObject{std::move(type1a)};
}

TEST_F(ObjectTest, moveConstructFromReferenceObject_notInvokeMoveConstructor)
{
    EXPECT_CALL(functions, default_constructor(_)); // Value ctor
    Type1 value;
    Object referenceObject{Type::get<Type1>(), &value, Object::ConstructReference};
    EXPECT_CALL(functions, move_constructor(_, &value)).Times(0);
    Object moveOfReferenceObject{std::move(referenceObject)};
    EXPECT_TRUE(moveOfReferenceObject.isReference());
    EXPECT_EQ(moveOfReferenceObject.raw(), &value);
}

TEST_F(ObjectTest, copyAssignFromSameTypeObject_invoquesCopyAssignmentOperator)
{
    EXPECT_CALL(functions, copy_assignment(type1a.raw(), type1b.raw()));
    type1a = type1b;
}

TEST_F(ObjectTest, moveAssignFromSameTypeObject_invoquesMoveAssignmentOperator)
{
    EXPECT_CALL(functions, move_assignment(type1a.raw(), type1b.raw()));
    type1a = std::move(type1b);
}

TEST_F(ObjectTest, copyAssignFromDifferentTypeObject_destroysPreviousValueAndCopyConstructsNew)
{
    EXPECT_CALL(functions, destructor(type1a.raw()));
    EXPECT_CALL(functions, copy_constructor(_, type2a.raw()));
    type1a = type2a;
}

TEST_F(ObjectTest, moveAssignFromDifferentTypeObject_destroysPreviousValueAndMoveConstructsNew)
{
    EXPECT_CALL(functions, destructor(type1a.raw()));
    EXPECT_CALL(functions, move_constructor(_, type2a.raw()));
    type1a = std::move(type2a);
}

TEST_F(ObjectTest, destructorOfReferenceObject_notInvokeDestructor)
{
    EXPECT_CALL(functions, default_constructor(_)); // value ctor
    Type1 value;

    {
        SpecialFunctions functions;
        Type1::reset(&functions);

        Object referenceObject{Type::get<decltype(value)>(), &value, Object::ConstructReference};
        EXPECT_CALL(functions, destructor(&value)).Times(0); // 1 (value), not 2
    }

    Type1::reset(&functions);
}
