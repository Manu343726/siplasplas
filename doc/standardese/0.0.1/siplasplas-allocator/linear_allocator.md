---
layout: standardese-doc
---

# Header file `linear_allocator.hpp`

``` cpp
#include "detail/track_top_allocator.hpp"

namespace cpp
{
    class LinearAllocator
    {
    public:
        LinearAllocator() = default;
        
        LinearAllocator(char* begin, char* end);
        
        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0);
        
        std::string dump() const;
        
        void deallocate(void* ptr, std::size_t count, std::size_t offset = 0);
    };
}
```
