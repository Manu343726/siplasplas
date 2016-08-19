---
layout: standardese-doc
---

# Header file `string.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_STRING_HPP 

#include <string>

#include <vector>

#include <siplasplas/utility/export.hpp>

namespace cpp
{
    std::vector<std::string> split(const std::string& string, const std::string& delimiter = "");
}
```
