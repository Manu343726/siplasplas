---
layout: standardese-doc
---

# Header file `function_traits.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_FUNCTION_TRAITS_HPP 

#include "meta.hpp"

#include <type_traits>

#include <ctti/type_id.hpp>

namespace cpp
{
    enum class FunctionKind;
    
    namespace detail
    {
        template <typename T>
        struct IsFunctorClass;
        
        template <typename Function>
        struct get_function_signature;
        
        template <typename R, typename ... Args>
        struct get_function_signature<R(Args...)>;
        
        template <typename R, typename ... Args>
        struct get_function_signature<R(*)(Args...)>;
        
        template <typename C, typename R, typename ... Args>
        struct get_function_signature<R (C::*)(Args...)>;
        
        template <typename C, typename R, typename ... Args>
        struct get_function_signature<R (C::*)(Args...) const>;
        
        template <typename T, typename Class>
        struct get_function_signature<T Class::*>;
    }
    
    template <typename Function, bool IsFunctor = detail::IsFunctorClass<Function>::value>
    struct function_signature;
    
    template <typename Functor>
    struct function_signature<Functor, true>;
    
    template <typename Function>
    constexpr FunctionKind function_kind();
    
    template <typename Function>
    constexpr FunctionKind function_kind(Function);
    
    template <typename A, typename B>
    struct equal_signature;
}
```
