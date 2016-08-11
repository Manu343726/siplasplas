---
layout: standardese-doc
---

# Header file `destroy.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_DESTROY_HPP 

namespace cpp
{
    template <typename T>
    void destroy(T& object);
    
    template <typename T>
    void destroy(T* objectPtr);
}
```
