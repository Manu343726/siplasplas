#ifndef SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_HANDLETEST_HPP
#define SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_HANDLETEST_HPP

#include <siplasplas/reflection/parser/api/core/clang/handle.hpp>
#include "mocks.hpp"

namespace cpp
{

namespace test
{

class CustomRawHandleType
{
public:
    constexpr CustomRawHandleType() :
        CustomRawHandleType{0}
    {}

    constexpr CustomRawHandleType(int value) :
        value{value}
    {}

    static constexpr int null = 42;

    friend constexpr bool operator==(const CustomRawHandleType& lhs, const CustomRawHandleType& rhs)
    {
        return lhs.value == rhs.value;
    }

    friend constexpr bool operator!=(const CustomRawHandleType& lhs, const CustomRawHandleType& rhs)
    {
        return !(lhs == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const CustomRawHandleType& rawHandle)
    {
        if(rawHandle.value == CustomRawHandleType::null)
        {
            return os << "CustomRawHandleType(" << rawHandle.value << ") (null)";
        }
        else
        {
            return os << "CustomRawHandleType(" << rawHandle.value << ")";
        }
    }

    int value;
};

template<typename TestClass>
class HandleTest : public ::testing::Test
{
protected:
    decltype(auto) expectRelease(const CustomRawHandleType& rawHandle)
    {
        return EXPECT_CALL((mocks::ReleaseHandleMock<CustomRawHandleType, TestClass>::get()), release(rawHandle));
    }

    decltype(auto) expectRelease(int rawHandleValue)
    {
        return expectRelease(CustomRawHandleType{rawHandleValue});
    }

    ~HandleTest() override
    {
        ::testing::Mock::VerifyAndClear(&mocks::ReleaseHandleMock<CustomRawHandleType, TestClass>::get());
    }
};

}

}

namespace cpp
{

namespace reflection
{

namespace parser
{

namespace api
{

namespace core
{

namespace clang
{

template<>
class NullHandle<::cpp::test::CustomRawHandleType>
{
public:
    static constexpr ::cpp::test::CustomRawHandleType value = ::cpp::test::CustomRawHandleType{::cpp::test::CustomRawHandleType::null};
};

}

}

}

}

}

}

#endif // SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_HANDLETEST_HPP
