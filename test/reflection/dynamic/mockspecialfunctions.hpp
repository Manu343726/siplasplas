#ifndef SIPLASPLAS_TEST_REFLECTION_DYNAMIC_MOCKSPECIALFUNCTIONS_HPP
#define SIPLASPLAS_TEST_REFLECTION_DYNAMIC_MOCKSPECIALFUNCTIONS_HPP

#include <gmock/gmock.h>
#include <siplasplas/utility/fusion.hpp>
#include <siplasplas/utility/meta.hpp>

using namespace ::testing;

class SpecialFunctions
{
public:
    MOCK_METHOD1(destructor, void(const void*));
    MOCK_METHOD1(default_constructor, void(const void*));
    MOCK_METHOD2(copy_constructor, void(const void*, const void*));
    MOCK_METHOD2(move_constructor, void(const void*, const void*));
    MOCK_METHOD2(copy_assignment, void(const void*, const void*));
    MOCK_METHOD2(move_assignment, void(const void*, const void*));
};

template<typename T>
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

class MockSpecialFunctionsTest
{
protected:
    template<typename... Ts>
    MockSpecialFunctionsTest(const std::vector<void*>&  objects, Ts...)
    {
        cpp::foreach_type<Ts...>([this](auto type)
        {
            using Type = cpp::meta::type_t<cpp::meta::type_t<decltype(type)>>;

            Type::reset(&functions);
        });

        for(void* object : objects)
        {
            if(object != nullptr)
            {
                EXPECT_CALL(functions, default_constructor(object));
                EXPECT_CALL(functions, destructor(object));
            }
            else
            {
                EXPECT_CALL(functions, default_constructor(_)).Times(AtLeast(0));
                EXPECT_CALL(functions, destructor(_)).Times(AtLeast(0));
            }
        }
    }

    SpecialFunctions functions;
};


#endif // SIPLASPLAS_TEST_REFLECTION_DYNAMIC_MOCKSPECIALFUNCTIONS_HPP
