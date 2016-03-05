
#include <gmock/gmock.h>
#include <vector>

#include <siplasplas/allocator/linear_allocator.hpp>
#include <siplasplas/allocator/stl_allocator.hpp>

using namespace ::testing;
using namespace ::cpp;

template<typename T>
using Vector = std::vector<T, STLAllocator<T, LinearAllocator>>;

static constexpr std::size_t TestBegin = 0;
static constexpr std::size_t TestEnd = 128;
static constexpr std::size_t TestLength = TestEnd - TestBegin;
static constexpr std::size_t TestBufferLength = TestLength * 1000;

class LinearAllocatorTest : public TestWithParam<std::size_t>
{
protected:
    using T = int;

    LinearAllocatorTest() :
        buffer( TestBufferLength*sizeof(T) ),
        v{LinearAllocator{begin(), end()}}
    {}

	virtual ~LinearAllocatorTest() = default;

    std::size_t max_elements() const
    {
        return TestBufferLength;   
    }

    char* begin()
    {
        return &buffer[0];
    }

    char* end()
    {
        return begin() + TestBufferLength*sizeof(T);
    }

	std::vector<char> buffer;
    Vector<T> v;
};

TEST_P(LinearAllocatorTest, defaultConstruction)
{
    auto f = [this]{
        Vector<int> v{LinearAllocator{begin(), end()}};
    };
    EXPECT_NO_THROW(f());
}

TEST_P(LinearAllocatorTest, pushNoThrow)
{
    for(std::size_t i = TestBegin; i < TestEnd; ++i)
    {
        SCOPED_TRACE(v.get_allocator().dump());
        EXPECT_NO_THROW(v.push_back(i));
    }
}

INSTANTIATE_TEST_CASE_P(
    LinearAllocatorMainTest,
    LinearAllocatorTest,
    Range(TestBegin, TestEnd)
);

