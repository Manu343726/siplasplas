---
layout: standardese-doc
---

# Header file `visitor.hpp`

``` cpp
#include <siplasplas/utility/function.hpp>

namespace cpp
{
    namespace {}
    
    template <typename R>
    struct VariantVisitorTraits;
    
    template <typename R, typename F, typename = void>
    struct VariantVisitor;
    
    template <typename R, typename F>
    struct VariantVisitor<R, F, void_t<typename F::ResultType>;
    
    template <typename R, typename ... Fs>
    VariantVisitor<R, Function<std::decay_t<Fs>...>> visitor(Fs&&... fs);
}
```
