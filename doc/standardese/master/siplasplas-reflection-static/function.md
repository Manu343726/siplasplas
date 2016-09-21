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
            class Function<SourceInfo_, R(*)(Args...), function>
            {
            public:
                using type = R(*)(Args...);
                
                using SourceInfo = SourceInfo_;
                
                constexpr Function() = default;
                
                template <typename ... Args_>
                static constexpr R invoke(Args_&&... args);
                
                static constexpr type get();
                
                template <typename ... Args_>
                constexpr R operator()(Args_&&... args) const;
            };
            
            template <typename SourceInfo_, typename R, typename Class, typename ... Args, R(Class::*)(Args...)const method>
            class Function<SourceInfo_, R(Class::*)(Args...)const, method>
            {
            public:
                using type = R(Class::*)(Args...)const;
                
                using class_type = Class;
                
                using SourceInfo = SourceInfo_;
                
                constexpr Function() = default;
                
                template <typename ... Args_>
                static constexpr R invoke(const Class& object, Args_&&... args);
                
                static constexpr type get();
                
                template <typename ... Args_>
                constexpr R operator()(const Class& object, Args_&&... args) const;
            };
            
            template <typename SourceInfo_, typename R, typename Class, typename ... Args, R(Class::*)(Args...) method>
            class Function<SourceInfo_, R(Class::*)(Args...), method>
            {
            public:
                using type = R(Class::*)(Args...);
                
                using class_type = Class;
                
                using SourceInfo = SourceInfo_;
                
                constexpr Function() = default;
                
                template <typename ... Args_>
                static constexpr R invoke(Class& object, Args_&&... args);
                
                template <typename ... Args_>
                static constexpr R invoke(const Class& object, Args_&&... args);
                
                static constexpr type get();
                
                template <typename ... Args_>
                constexpr R operator()(const Class& object, Args_&&... args) const;
                
                template <typename ... Args_>
                constexpr R operator()(Class& object, Args_&&... args) const;
            };
        }
        
        namespace codegen
        {
            template <typename FunctionType, FunctionType function>
            class Function{};
            
            template <typename Head>
            class OverloadedFunction<Head>
            : public Head
            {};
            
            template <typename Head, typename Second, typename ... Tail>
            class OverloadedFunction<Head, Second, Tail...>
            : public Head, public OverloadedFunction<Second, Tail...>
            {
            public:
                using class_type = typename Head::class_type;
                
                using Head::invoke;
                
                using Head::operator();
                
                using OverloadedFunction<Second, Tail...>::invoke;
                
                using OverloadedFunction<Second, Tail...>::operator();
            };
            
            template <typename Method>
            class BindedMethod
            {
            public:
                BindedMethod(typename Method::class_type& object);
                
                template <typename ... Args>
                decltype(Method::invoke(*std::declval<typename Method::class_type*>(), std::forward<Args>(args)...)) operator()();
            };
            
            template <typename Method>
            class ConstBindedMethod
            {
            public:
                ConstBindedMethod(const typename Method::class_type& object);
                
                template <typename ... Args>
                decltype(Method::invoke(*std::declval<const typename Method::class_type*>(), std::forward<Args>(args)...)) operator()();
            };
        }
        
        template <typename FunctionType, FunctionType function>
        class Function;
    }
}
```

## Class template `cpp::static_reflection::Function<FunctionType, function>`<a id="cpp::static_reflection::Function__FunctionType, function__"></a>

``` cpp
template <typename FunctionType, FunctionType function>
class Function{};
```

Returns static reflection information of a function

See cpp::static\_reflection::meta::Function for the specific information returned

### Template parameter `cpp::static_reflection::Function<FunctionType, function>::function`<a id="cpp::static_reflection::Function__FunctionType, function__.function"></a>

``` cpp
FunctionType function
```

Pointer to the function

-----
