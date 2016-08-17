---
layout: standardese-doc
---

# Header file `invoke.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_INVOKE_HPP 

#include "function_traits.hpp"

#include "logger.hpp"

#include <utility>

#include <functional>

#include <iostream>

namespace cpp
{
    namespace detail
    {
        template <typename Class, typename Object, bool IsBase = std::is_base_of<Class, Object>::value>
        class InvokeMemberFunction;
        
        template <typename Class, typename T>
        class InvokeMemberFunction<Class, std::reference_wrapper<T>, false>;
        
        template <typename Class, typename Object>
        class InvokeMemberFunction<Class, Object, false>;
        
        template <typename R, typename Class, typename ... Params, typename Head, typename ... Tail>
        auto invokeMemberFunction(R (Class::*)(Params...) function, Head&& head, Tail&&... tail);
        
        template <typename R, typename Class, typename ... Params, typename Head, typename ... Tail>
        auto invokeMemberFunction(R (Class::*)(Params...) const function, Head&& head, Tail&&... tail);
    }
    
    template <typename Callable, typename ... Args>
    auto invoke(Callable&& callable, Args&&... args);
    
    template <typename R, typename Class, typename ... Params, typename ... Args>
    auto invoke(R (Class::*)(Params...) function, Args&&... args);
    
    template <typename R, typename Class, typename ... Params, typename ... Args>
    auto invoke(R (Class::*)(Params...) const function, Args&&... args);
}
```
