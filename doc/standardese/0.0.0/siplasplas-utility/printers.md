---
layout: standardese-doc
---

# Header file `printers.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_PRINTERS_HPP 

#include <ostream>

#include <functional>

#include <siplasplas/utility/export.hpp>

template <typename F>
decltype(f(os)) operator<<(std::ostream& os, F f);

namespace cpp
{
    
    std::function<std::ostream&(std::ostream&)> print_memory(std::ostream&, const char* begin, const char* end, std::size_t word = 0, char byteSeparator = 0);
}
```
