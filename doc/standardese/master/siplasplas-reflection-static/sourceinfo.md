
---
layout: standardese-doc
---

# Header file `sourceinfo.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP 

#include <siplasplas/utility/meta.hpp>

#include <ctti/type_id.hpp>

#include <array>

#include <string>

namespace cpp
{
    namespace static_reflection
    {
        enum class Kind;
        
        namespace meta
        {
            namespace 
            {
                template <typename T>
                struct FullNameFromType;
            }
        }
    }
}
```
