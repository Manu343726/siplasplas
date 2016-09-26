---
layout: standardese-doc
---

# Header file `lifo_allocator.hpp`

``` cpp
#include "detail/track_top_allocator.hpp"

namespace cpp
{
    class LifoAllocator
    : protected cpp::TrackTopAllocator
    {
    public:
        using offset_t = std::uint8_t;
        
        LifoAllocator(void* begin, void* end);
        
        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0);
        
        void deallocate(void* pointer, std::size_t size);
        
        std::string dump() const;
    };
}
```
