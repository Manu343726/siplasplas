---
layout: standardese-doc
---

# Header file `staticif.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_STATICIF_HPP 

#include "type_variables.hpp"

namespace cpp
{
    namespace detail
    {
        class Identity;
        
        template <bool Condition>
        class If;
        
        template <>
        class If<false>;
    }
    
    template <bool Condition, typename ThenBody>
    auto staticIf(const ThenBody& thenBody);
}
```

## Class `cpp::detail::Identity`<a id="cpp::detail::Identity"></a>

``` cpp
class Identity
{
public:
    template <typename T>
    constexpr decltype(std::forward<T>(value)) operator()();
    
    template <typename T, typename Function>
    constexpr decltype(callback(type<T>())) type(Function callback);
};
```

A functor class implementing the identity function

The identity function takes a value of any type and returns it as is. The function performs no mutation of the value. Given an expression `x` `decltype(x) == decltype(Identity()(x))` and the yield value is the same. This function is useful to delay the evaluation of an expression to the second template processing phase (The on instantiation phase). See cpp::staticIf.

-----

## Class template `cpp::detail::If<Condition>`<a id="cpp::detail::If<Condition>"></a>

``` cpp
template <bool Condition>
class If
{
public:
    template <typename T>
    class ElseBypass;
    
    template <typename T>
    class ElseBypass<T&>;
    
    template <typename Body>
    constexpr typename std::enable_if<
    !std::is_void<decltype(std::declval<Body>()(Identity()))>::value,
    ElseBypass<decltype(std::declval<Body>()(Identity()))>
    >::type Then(const Body& body);
    
    template <typename Body>
    constexpr typename std::enable_if<
    std::is_void<decltype(std::declval<Body>()(Identity()))>::value
    >::type Then(const Body& body);
    
    template <typename Body>
    constexpr void Else(const Body&);
};
```

Implements the then branch of an static conditional.

The cpp::detail::If template implements the internals of an `static_if()`. The main template implements the true condition branch (The branch executing the then body). Returning values in this case may not be as efficient as possible since the returned value should bypass the then body (This involves two move operations, see cpp::If::ElseBypass):

``` cpp
std::string str = If<true>().Then([](auto identity)
{
    return "hello, world!"s; // the string is moved twice
                             // to exit the static if.
});
```

### Template parameter `cpp::detail::If<Condition>::Condition`<a id="cpp::detail::If<Condition>::Condition"></a>

``` cpp
bool Condition
```

If condition. True in the main template.

-----

## Function template `cpp::staticIf<Condition, ThenBody>`<a id="cpp::staticIf<Condition, ThenBody>"></a>

``` cpp
template <bool Condition, typename ThenBody>
auto staticIf(const ThenBody& thenBody);
```

An static conditional allows to conditionally evaluate some code depending on the value of a compile time property. The body of the conditional is implemented by user provided functions.

``` cpp
template<typename T>
void f(T value)
{
    cpp::staticIf<std::is_integral<T>::value>([&](auto identity)
    {
        std::cout << "the value is integral\n";
    });

    ...
}
```

The body of the untaken conditional path shall not be evaluated by the compiler and can contain ill formed code. **NOTE**: This behavior relies on the two phase template processing scheme, so the statement above is only true for entities that will be evaluated in the instantiation phase only:

``` cpp
template<typename T>
void foo(T value)
{
   int i = 0;

   // The condition is false, so the code
   // inside the if "should" not be evaluated:
   cpp::staticIf<false>([&](auto identity)
   {
       // Ok: value depends on T template parameter.
       value.TheMostBizarreMethodName();

       // ERROR: 'int' type is not class/struct/union.
       i.method();

       // Ok: identity(i) depends on a template parameter.
       identity(i).method();
   });
}
```

As the example shows, the conditional body takes an `identity` parameter that can be used to force the evaluation of an expression at the second phase.

The static conditional expression also provides an `else` sentence in the form of an `Else()` method:

``` cpp
template<typename T>
void foo(T value)
{
   int i = 0;

   cpp::staticIf<false>([&](auto identity)
   {
       value.TheMostBizarreMethodName();
       i.method();
       identity(i).method();
   }).Else([&](auto identity)
   {
       std::cout << i << std::endl;
   });
}
```

cpp::staticIf() supports returning values from the conditional body too:

``` cpp
template<typename T>
T twice(const T& value)
{
    return cpp::staticIf<std::is_class<T>::value>([&](auto)
    {
        return value.twice();
    }).Else([&](auto)
    {
        return value * 2;
    });
}
```

note this has some caveats:

  - **Return types cannot be inferred**: Due to technical reasons `auto` or any other kind of type inference cannot be used with the return value of a static conditional.

  - **Return may not be optimal in some code paths**: If the condition is true, the `then` path is picked. If the `then` body returns a value, such value is not returned directly (So elligible for RVO) but bypassed through the `else` internals. This means **returning a value from a positive conditional may involve two move operations**.

### Template parameter `cpp::staticIf<Condition, ThenBody>::Condition`<a id="cpp::staticIf<Condition, ThenBody>::Condition"></a>

``` cpp
bool Condition
```

Value of the condition. The value should be evaluable at compile time, else compilation fails. \\tparam ThenBody Function type with one template-dependent value parameter. \\param thenBody Body of the `then` path of the conditional. Evaluated only if the condition is true. \\return An unspecified type implementing the `else` part of the conditional.

-----
