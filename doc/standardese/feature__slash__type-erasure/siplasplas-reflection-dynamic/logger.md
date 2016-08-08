---
layout: standardese-doc
---

# Header file `logger.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_DYNAMIC_LOGGER_HPP 

#include <siplasplas/logger/logger.hpp>

#include <siplasplas/reflection/dynamic/export.hpp>

namespace cpp
{
    namespace reflection
    {
        namespace dynamic
        {
             spdlog::logger& log();
        }
    }
}
```
