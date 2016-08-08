---
layout: standardese-doc
---

# Header file `function.hpp`

``` cpp
#define SIPLASPLAS_REFLECTION_STATIC_FUNCTION_HPP 

namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename R, typename ... Args, R(*function)(Args...)>
            class Function<SourceInfo_, R(*)(Args...), function>;
            
            template <typename SourceInfo_, typename R, typename Class, typename ... Args, R(Class::*)(Args...) const method>
            class Function<SourceInfo_, R(Class::*)(Args...) const, method>;
            
            template <typename SourceInfo_, typename R, typename Class, typename ... Args, R(Class::*)(Args...) method>
            class Function<SourceInfo_, R(Class::*)(Args...), method>;
        }
        
        namespace codegen
        {
            template <typename FunctionType, FunctionType function>
            class Function;
            
            template <typename Head>
            class OverloadedFunction<Head>;
            
            template <typename Head, typename Second, typename ... Tail>
            class OverloadedFunction<Head, Second, Tail...>;
            
            template <typename Method>
            class BindedMethod;
            
            template <typename Method>
            class ConstBindedMethod;
        }
        
        template <typename FunctionType, FunctionType function>
        class Function;
    }
}
```
