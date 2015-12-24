#ifndef SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP
#define SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

#include <memory>

namespace cpp
{
    namespace detail
    {
        char* aligned_ptr(char* pointer, std::size_t alignment)
        {
            return pointer + (reinterpret_cast<std::uintptr_t>(pointer) % alignment);
        }
    }
}

#endif // SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

