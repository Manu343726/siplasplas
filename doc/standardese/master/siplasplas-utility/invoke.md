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
        
        template <typename Class, typename Object, bool IsBase = std::is_base_of<Class, Object>::value>
        class InvokeMemberObject;
        
        template <typename Class, typename T>
        class InvokeMemberObject<Class, std::reference_wrapper<T>, false>;
        
        template <typename Class, typename Object>
        class InvokeMemberObject<Class, Object, false>;
        
        template <typename T, bool IsMemberObjectPointer = std::is_member_object_pointer<T>::value>
        class InvokeMember;
        
        template <typename T>
        class InvokeMember<T, false>;
    }
    
    template <typename Callable, typename ... Args>
    auto invoke(Callable&& callable, Args&&... args);
    
    template <typename R, typename Class, typename ... Args>
    auto invoke(R Class::* pointer, Args&&... args);
    
    template <typename R, typename Class, typename ... Params, typename ... Args>
    auto invoke(R (Class::*)(Params...) const pointer, Args&&... args);
}
```
