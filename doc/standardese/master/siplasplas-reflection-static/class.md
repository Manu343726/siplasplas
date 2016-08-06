---
layout: standardese-doc
---

# Header file `class.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP 

#include "sourceinfo.hpp"

#include <siplasplas/utility/meta.hpp>

namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo, typename Class_, typename Methods_, typename Fields_, typename Constructors_, typename Classes_, typename Enums_>
            class Class;
        }
        
        namespace codegen
        {
            template <typename T>
            class Class;
        }
        
        template <typename T>
        class Class;
    }
}
```
