#ifndef SIPLASPLAS_ALLOCATOR_FREELIST_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_FREELIST_ALLOCATOR_HPP

#include <string>

#include "detail/embedded_allocator.hpp"
#include <siplasplas/allocator/export.hpp>

namespace cpp
{
    class SIPLASPLAS_ALLOCATOR_EXPORT FreeListAllocator : public EmbeddedAllocator 
    {
    public:
        struct FreeListNode
        {
            FreeListNode* next;
        };

        cpp::detail::RawReaderWriter<FreeListNode*> head() const;
        
        FreeListAllocator(void* begin, void* end, std::size_t block_length, std::size_t alignment, std::size_t offset = 0);

        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0);
        void deallocate(void* pointer, std::size_t size, std::size_t offset = 0);

        std::string dump() const;

    private:
        cpp::detail::RawReaderWriter<FreeListNode*> head();
    };
}

#endif // SIPLASPLAS_ALLOCATOR_FREELIST_ALLOCATOR_HPP
