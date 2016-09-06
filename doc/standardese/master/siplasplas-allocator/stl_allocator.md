---
layout: standardese-doc
---

# Header file `stl_allocator.hpp`

``` cpp
#include <memory>

#include <ctti/type_id.hpp>

#include <sstream>

namespace cpp
{
    template <typename T, typename Allocator>
    struct STLAllocator;
    
    template <typename T, typename Allocator>
    cpp::STLAllocator<T, std::decay_t<Allocator>> make_stl_allocator();
}
```
