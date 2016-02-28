#ifndef SIPLASPLAS_ALLOCATOR_CANARY_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_CANARY_ALLOCATOR_HPP

#include <cstdint>
#include <algorithm>

namespace cpp
{
    template<typename Alloc>
    class CanaryAllocator : public Alloc
    {
    public:
        template<typename... Args>
        CanaryAllocator(Args&&... args) :
            Alloc(std::forward<Args>(args)...)
        {}

        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
        {
            char* user_ptr = reinterpret_cast<char*>(
                Alloc::allocate(size + 4, alignment, 4 + offset)
            );
            
            std::fill(user_ptr - offset - 4, user_ptr - offset, 0xf);
            std::fill(user_ptr, user_ptr + size, 0xc);
            std::fill(user_ptr + size, user_ptr + size + 4, 0xf);
            
            return user_ptr;
        }
        
        void deallocate(void* pointer, std::size_t size)
        {
            Alloc::deallocate(pointer, size + 4);   
        }
    };
}

#endif // SIPLASPLAS_ALLOCATOR_CANARY_ALLOCATOR_HPP
