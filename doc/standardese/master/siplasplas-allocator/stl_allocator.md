---
layout: standardese-doc
---

# Header file `stl_allocator.hpp`

``` cpp
namespace cpp
{
    template <typename T, typename Allocator>
    struct STLAllocator;
    
    template <typename T, typename Allocator>
    cpp::STLAllocator<T, std::decay_t<Allocator>> make_stl_allocator();
}
```
