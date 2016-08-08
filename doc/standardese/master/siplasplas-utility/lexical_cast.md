---
layout: standardese-doc
---

# Header file `lexical_cast.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_LEXICAL_CAST_HPP 

#include <sstream>

namespace cpp
{
    template <typename T>
    std::string lexical_cast(const T& value);
    
    template <typename T>
    T lexical_cast(const std::string& value);
}
```

## Function template `cpp::lexical_cast<T>`<a id="cpp::lexical_cast<T>"></a>

``` cpp
template <typename T>
std::string lexical_cast(const T& value);
```

This function invokes `operator<<(std::ostream&, const T&)` to get an string representation of a given value.

It's a simplified version of the [Boost.Lexical\_Cast library](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_lexical_cast.html).

@return A string representation of the given value.

### Template parameter `cpp::lexical_cast<T>::T`<a id="cpp::lexical_cast<T>::T"></a>

``` cpp
typename T
```

value type. Must have a valid `operator<<(std::ostream&, T)` overload. \\param value Value which is translated to a string.

-----

## Function template `cpp::lexical_cast<T>`<a id="cpp::lexical_cast<T>"></a>

``` cpp
template <typename T>
T lexical_cast(const std::string& value);
```

This function invokes `operator>>(std::istream&, T&)` to assign a T value from an string representation of it.

It's a simplified version of the [Boost.Lexical\_Cast library](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_lexical_cast.html).

@return A value of type \\p T equivalent to the \\p value string representation argument.

### Template parameter `cpp::lexical_cast<T>::T`<a id="cpp::lexical_cast<T>::T"></a>

``` cpp
typename T
```

Required output value type. Must have a valid `operator>>(std::istream&, T&)` overload. \\param value String representation of the value.

-----
