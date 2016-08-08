---
layout: standardese-doc
---

# Header file `visitor.hpp`

``` cpp
#define SIPLASPLAS_VARIANT_VISITOR_HPP 

#include <siplasplas/utility/function.hpp>

namespace cpp
{
    namespace {}
    
    template <typename R>
    struct VariantVisitorTraits;
    
    template <typename R, typename F, typename = void>
    struct VariantVisitor;
}
```
