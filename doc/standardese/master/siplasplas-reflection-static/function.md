---
layout: standardese-doc
---

# Header file `function.hpp`

``` cpp
namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename R, typename ... Args, R(*function)(Args...)>
            class Function<SourceInfo_, R(*)(Args...), function>;
            
            template <typename SourceInfo_, typename R, typename Class, typename ... Args, R(Class::*)(Args...)const method>
            class Function<SourceInfo_, R(Class::*)(Args...)const, method>;
            
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

## Class template `cpp::static_reflection::Function<FunctionType, function>`<a id="cpp::static_reflection::Function<FunctionType, function>"></a>

``` cpp
template <typename FunctionType, FunctionType function>
class Function{};
```

Returns static reflection information of a function

See cpp::static\_reflection::meta::Function for the specific information returned

### Template parameter `cpp::static_reflection::Function<FunctionType, function>::function`<a id="cpp::static_reflection::Function<FunctionType, function>.function"></a>

``` cpp
FunctionType function
```

Pointer to the function

-----
