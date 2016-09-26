---
layout: standardese-doc
---

# Header file `freelist_allocator.hpp`

``` cpp
#include "detail/embedded_allocator.hpp"

namespace cpp
{
    class FreeListAllocator
    : public cpp::EmbeddedAllocator
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
    };
}
```
