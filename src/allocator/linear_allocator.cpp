
#include "siplasplas/allocator/linear_allocator.hpp"

using namespace cpp;

LinearAllocator::LinearAllocator(char* begin, char* end) :
    IntrusiveAllocator{ begin, end }
{}

void* LinearAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
{
    // Take the first address available (_top) plus the offset, then adjust
    // it to the required boundary
    char* user_ptr = detail::aligned_ptr(top() + offset, alignment);
    char* block_end = user_ptr + size;

    if(block_end <= end())
    {
        set_top(block_end);
        return user_ptr;
    }
    else
    {
        // Out of space
        return nullptr;
    }
}

void LinearAllocator::deallocate(void* ptr, std::size_t count, std::size_t offset)
{
    // nop
}

std::string LinearAllocator::dump() const
{
    return IntrusiveAllocator::dump();
}

