#ifndef SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_MOCKS_HPP
#define SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_MOCKS_HPP

#include <gmock/gmock.h>
#include <siplasplas/reflection/parser/api/core/clang/handle.hpp>

namespace cpp
{

namespace test
{

namespace mocks
{

template<typename RawHandle, typename Tag>
class ReleaseHandleMock
{
public:
    MOCK_CONST_METHOD1_T(release, void(RawHandle));

    static ReleaseHandleMock& get()
    {
        static ReleaseHandleMock mock;
        return mock;
    }
};

template<typename RawHandle, typename Tag>
void releaseHandle(RawHandle handle)
{
    ReleaseHandleMock<RawHandle, Tag>::get().release(handle);
}

}

}

}

#endif // SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_CLANG_MOCKS_HPP
