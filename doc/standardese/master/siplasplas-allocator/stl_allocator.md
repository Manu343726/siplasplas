
---
layout: standardese-doc
---

# Header file `stl_allocator.hpp`

``` cpp
#define SIPLASPLAS_ALLOCATOR_STL_ALLOCATOR_HPP 

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
