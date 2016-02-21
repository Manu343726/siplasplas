
#include "siplasplas/allocator/linear_allocator.hpp"

using namespace cpp;

LinearAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
{
    assert(offset == 0 && "Cannot do offsetting with this allocator, it stores metadata before user block-address");

    offset = sizeof(offset_t);

    char* user_ptr = detail::aligned_ptr(top() + offset, alignment);
    char* block_end = user_ptr + size;

    if (block_end <= end())
    {
        // Store padding length between current top and returned (user) ptr 
        detail::write_before(user_ptr, static_cast<offset_t>(user_ptr - top()));

        set_top(block_end);

        return user_ptr;
    }
    else
    {
        return nullptr;
    }
}

