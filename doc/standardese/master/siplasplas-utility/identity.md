---
layout: standardese-doc
---

# Header file `identity.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_IDENTITY_HPP 

#include "meta.hpp"

#include <utility>

namespace cpp
{
    class Identity;
}
```

## Class `cpp::Identity`<a id="cpp::Identity"></a>

``` cpp
class Identity
{
public:
    template <typename T>
    constexpr decltype(std::forward<T>(value)) const operator()(T&& value);
    
    template <typename T, typename Function>
    constexpr decltype(callback(meta::identity<T>())) const type(Function callback);
    
    template <typename T>
    constexpr auto type() const;
};
```

The identity function takes a value of any type and returns it as is. The function performs no mutation of the value. Given an expression `x` `decltype(x) == decltype(Identity()(x))` and the yield value is the same. This function is useful to delay the evaluation of an expression to the second template processing phase (The on instantiation phase). See cpp::staticIf or cpp::compiles.

-----
