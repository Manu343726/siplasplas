#include "siplasplas/allocator/allocator_utils.hpp"

namespace cpp
{

namespace detail
{

char* aligned_ptr(char* pointer, std::size_t alignment)
{
    assert(alignment > 0 && (alignment & (alignment - 1)) == 0 && "Must align to a power of two boundary");

    auto addr = reinterpret_cast<std::uintptr_t>(pointer);
    char* result = (addr % alignment != 0) ?
        pointer + alignment - (addr % alignment) :
        pointer;

    assert(result >= pointer && "Aligned pointer should be greater");

    return result;
}

void* aligned_ptr(void* pointer, std::size_t alignment)
{
    return aligned_ptr(reinterpret_cast<char*>(pointer), alignment);
}

}

}
