---
layout: standardese-doc
---

# Header file `track_top_allocator.hpp`

``` cpp
#include "embedded_allocator.hpp"

namespace cpp
{
    class TrackTopAllocator
    : protected cpp::EmbeddedAllocator
    {
    protected:
        TrackTopAllocator(void* begin, void* end);
        
        char* top() const;
        
        void set_top(void* pointer);
        
        void commit(std::size_t bytes);
        
        void decommit(std::size_t bytes);
        
        std::size_t bytes() const;
        
        std::size_t bytes_left() const;
        
        bool belongs_to_storage(void* pointer) const;
        
        bool full() const;
        
        std::string dump() const;
    };
}
```
