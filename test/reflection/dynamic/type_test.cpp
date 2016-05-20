#include <gmock/gmock.h>
#include <siplasplas/reflection/dynamic/type.hpp>
#include <iostream>

using namespace ::testing;
using namespace ::cpp;
using namespace ::cpp::dynamic_reflection;


class SpecialFunctionsBase
{
public:
    MOCK_METHOD1(destructor, void(const void*));
    MOCK_METHOD1(default_constructor, void(const void*));
    MOCK_METHOD2(copy_constructor, void(const void*, const void*));
    MOCK_METHOD2(move_constructor, void(const void*, const void*));
    MOCK_METHOD2(copy_assignment, void(const void*, const void*));
    MOCK_METHOD2(move_assignment, void(const void*, const void*));
};

class SpecialFunctions : public SpecialFunctionsBase
{};

class MockSpecialFunctions
{
public:
    MockSpecialFunctions()
    {
        specialFunctions().default_constructor(this);
    }

    MockSpecialFunctions(const MockSpecialFunctions& other) :
        value{other.value}
    {
        specialFunctions().copy_constructor(this, &other);
    }

    MockSpecialFunctions(MockSpecialFunctions&& other) :
        value{other.value}
    {
        specialFunctions().move_constructor(this, &other);
        other.value = "";
    }

    MockSpecialFunctions& operator=(const MockSpecialFunctions& other)
    {
        specialFunctions().copy_assignment(this, &other);
        value = other.value;
        return *this;
    }

    MockSpecialFunctions& operator=(MockSpecialFunctions&& other)
    {
        value = other.value;
        other.value = "";
        specialFunctions().move_assignment(this, &other);
        return *this;
    }

    ~MockSpecialFunctions()
    {
        specialFunctions().destructor(this);
    }

    static void reset(SpecialFunctions* functions)
    {
        *MockSpecialFunctions::functions() = functions;
    }

    static SpecialFunctions& specialFunctions()
    {
        return **functions();
    }

    std::string value = "hello, world!";

public:
    static SpecialFunctions& defaultFunctions()
    {
        static SpecialFunctions defaultFunctions;
        return defaultFunctions;
    }

    static SpecialFunctions** functions()
    {
        static SpecialFunctions* functions = &defaultFunctions();
        return &functions;
    }
};

class TypeTestBase
{
protected:
    TypeTestBase(void* object, void* other)
    {
        MockSpecialFunctions::reset(&functions);
        EXPECT_CALL(functions, default_constructor(object));
        EXPECT_CALL(functions, default_constructor(other));
        EXPECT_CALL(functions, destructor(object));
        EXPECT_CALL(functions, destructor(other));
    }

    Type type = Type::get<MockSpecialFunctions>();
    SpecialFunctions functions;
};

class TypeTest : public TypeTestBase, public Test
{
public:
    TypeTest() :
        TypeTestBase{&object, &other}
    {
        other.value = "42";
    }

    void cleanup(void* testObject)
    {
        EXPECT_CALL(functions, destructor(testObject));
        type.destroy(testObject);
    }

protected:
    MockSpecialFunctions object, other;
};

TEST_F(TypeTest, constructInvokesDefaultConstructor)
{
    EXPECT_CALL(functions, default_constructor(_));
    void* object = type.construct();

    cleanup(object);
}

TEST_F(TypeTest, copyConstructInvokesCopyConstructor)
{
    EXPECT_CALL(functions, copy_constructor(_, &other));
    void* object = type.copy_construct(&other);
    EXPECT_EQ(reinterpret_cast<MockSpecialFunctions*>(object)->value, "42");
    EXPECT_EQ(other.value, "42");

    cleanup(object);
}

TEST_F(TypeTest, moveConstructInvokesMoveConstructor)
{
    EXPECT_CALL(functions, move_constructor(_, &other));
    void* object = type.move_construct(&other);
    EXPECT_EQ(reinterpret_cast<MockSpecialFunctions*>(object)->value, "42");
    EXPECT_EQ(other.value, "");

    cleanup(object);
}

TEST_F(TypeTest, copyAssignInvokesCopyAssignmentOperator)
{
    EXPECT_CALL(functions, copy_assignment(&object, &other));
    type.copy_assign(&object, &other);
    EXPECT_EQ(object.value, "42");
    EXPECT_EQ(other.value, "42");
}

TEST_F(TypeTest, moveAssignmentInvokesMoveAssignmentOperator)
{
    EXPECT_CALL(functions, move_assignment(&object, &other));
    type.move_assign(&object, &other);
    EXPECT_EQ(object.value, "42");
    EXPECT_EQ(other.value, "");
}

TEST_F(TypeTest, destroyInvokesDestructor)
{
    EXPECT_CALL(functions, default_constructor(_));
    void* object = type.construct();
    EXPECT_CALL(functions, destructor(object));
    type.destroy(object);
}

TEST_F(TypeTest, typeWithNoLeftShiftOverload_toStringGivesDefaultMessage)
{
    std::ostringstream os;
    os << "'" << ctti::type_id<decltype(object)>().name() << "' instance @" << static_cast<void*>(&object);
    EXPECT_EQ(type.toString(&object), os.str());
}

TEST_F(TypeTest, typeWithNoRightShiftOverload_fromStringThrows)
{
    EXPECT_THROW(type.fromString("hello, world!"), std::runtime_error);
}

TEST_F(TypeTest, typeWithLeftShiftOverload_toStringNoThrow)
{
    int value = 0;
    std::string str;
    EXPECT_NO_THROW(str = Type::get<decltype(value)>().toString(&value));
    EXPECT_EQ(str, "0");
}

TEST_F(TypeTest, typeWithRightShiftOverload_fromStringNoThrow)
{
    int* integer = nullptr;
    ASSERT_NO_THROW(integer = reinterpret_cast<int*>(Type::get<int>().fromString("0")));
    EXPECT_EQ(*integer, 0);
    Type::get<int>().destroy(integer);
}
