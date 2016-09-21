---
layout: standardese-doc
---

# Header file `canary_allocator.hpp`

``` cpp
namespace cpp
{
    template <typename Alloc>
    class CanaryAllocator
    : public Alloc
    {
    public:
        template <typename ... Args>
        CanaryAllocator(Args&&... args);
        
        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0);
        
        void deallocate(void* pointer, std::size_t size);
    };
}
```
