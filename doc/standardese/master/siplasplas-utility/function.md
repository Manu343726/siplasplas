---
layout: standardese-doc
---

# Header file `function.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_FUNCTION_HPP 

#include "meta.hpp"

#include <utility>

namespace cpp
{
    template <typename F>
    struct Function<F>;
    
    template <typename R, typename ... Args>
    struct Function<R(Args...)>;
    
    template <typename First, typename Second, typename ... Tail>
    struct Function<First, Second, Tail...>;
    
    template <typename ... Fs>
    Function<cpp::meta::decay_t<Fs>...> make_function(Fs&&... fs);
}
```
