---
layout: standardese-doc
---

# Header file `universal_reference.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_UNIVERSALREFERENCE_HPP 

#include <type_traits>

#include <bitset>

namespace cpp
{
    template <typename T, bool IsLvalueReference, bool IsConst>
    class UniversalReference;
    
    template <typename T>
    class UniversalReference<
    T,
    true,
    false
    >;
    
    template <typename T>
    class UniversalReference<
    T,
    false,
    false
    >;
    
    template <typename T>
    UniversalReference<
    std::decay_t<T>,
    std::is_lvalue_reference<T>::value,
    std::is_const<std::remove_reference_t<T>>::value
    > universalReference(T&& value);
}
```

## Class template `cpp::UniversalReference<T, IsLvalueReference, IsConst>`<a id="cpp::UniversalReference<T, IsLvalueReference, IsConst>"></a>

``` cpp
template <typename T, bool IsLvalueReference, bool IsConst>
class UniversalReference
{
public:
    using ValueType = T;
    
    UniversalReference(const ValueType& lvalueRef);
    
    const ValueType& get() const;
};
```

This template and its specialization provides a common interface to store references to lvalue and rvalues. UniversalReference has different spacializations for each value category supported (reference to lvalue, const reference to lvalue, rvalue). Instances of this types should be created through cpp::universalReference() function, which instances the appropiate template given the input value category.

If the input value is an lvalue (i.e. the function parameter is a reference to an lvalue), UniversalReference specialization wrapping the reference is chosen. If the input value is an rvalue (the function parameter is a reference to an rvalue), the value is moved into the UniversalReference object and owned by it.

This main template is the specialization for const lvalues

### Function `cpp::UniversalReference<T, IsLvalueReference, IsConst>::get`<a id="cpp::UniversalReference<T, IsLvalueReference, IsConst>::get"></a>

``` cpp
const ValueType& get() const;
```

Returns a read only reference to the referenced object

-----

-----

## Class template `cpp::UniversalReference<
T,
true,
false
>`<a id="cpp::UniversalReference<
T,
true,
false
>"></a>

``` cpp
template <typename T>
class UniversalReference<
T,
true,
false
>
{
public:
    using ValueType = T;
    
    UniversalReference(ValueType& lvalueRef);
    
    ValueType& get() const;
};
```

See cpp::UniversalReference main template documentation for details

### Function `cpp::UniversalReference<
T,
true,
false
>::get`<a id="cpp::UniversalReference<
T,
true,
false
>::get"></a>

``` cpp
ValueType& get() const;
```

Returns a reference to the referenced object

-----

-----

## Class template `cpp::UniversalReference<
T,
false,
false
>`<a id="cpp::UniversalReference<
T,
false,
false
>"></a>

``` cpp
template <typename T>
class UniversalReference<
T,
false,
false
>
{
public:
    using ValueType = T;
    
    UniversalReference(ValueType&& rvalueRef);
    
    ValueType& get();
    
    const ValueType& get() const;
};
```

See cpp::UniversalReference main template documentation for details

### Function `cpp::UniversalReference<
T,
false,
false
>::get`<a id="cpp::UniversalReference<
T,
false,
false
>::get"></a>

``` cpp
ValueType& get();
```

Returns a reference to the referenced object

-----

### Function `cpp::UniversalReference<
T,
false,
false
>::get`<a id="cpp::UniversalReference<
T,
false,
false
>::get"></a>

``` cpp
const ValueType& get() const;
```

Returns a reference to the referenced object

-----

-----

## Function template `cpp::universalReference<T>`<a id="cpp::universalReference<T>"></a>

``` cpp
template <typename T>
UniversalReference<
std::decay_t<T>,
std::is_lvalue_reference<T>::value,
std::is_const<std::remove_reference_t<T>>::value
> universalReference(T&& value);
```

This function checks the value category of the given value and instances the apropiate cpp::UniversalReference specialization. Note the returned type depends on the value category. For an alternative with common a type, see cpp::typeerasure::AnyArg

*Returns:* A UniversalReference specialization instance as follows:

  - `UniversalReference<std::decay_t<T>, true, true>` if `value` type is deduced to a const reference to an lvalue.
  - `UniversalReference<std::decay_t<T>, true, false>` if `value` type is deduced to a non const reference to an lvalue.
  - `UniversalReference<std::decay_t<T>, false, false>` if `value` type is deduced to an rvalue reference

-----
