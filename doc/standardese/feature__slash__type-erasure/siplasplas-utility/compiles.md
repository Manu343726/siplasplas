---
layout: standardese-doc
---

# Header file `compiles.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_COMPILES_HPP 

#include "meta.hpp"

#include "identity.hpp"

#define SIPLASPLAS_UTILITY_COMPILES(...) \
        ::cpp::compiles(                                  \
            [](auto identity) -> decltype(__VA_ARGS__) {} \
        )

namespace cpp
{
    namespace detail
    {
        template <typename Expr>
        class Compiles;
    }
    
    template <typename Expr>
    constexpr auto compiles(Expr&&);
}
```
