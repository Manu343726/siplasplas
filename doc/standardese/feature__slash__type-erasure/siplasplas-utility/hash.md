---
layout: standardese-doc
---

# Header file `hash.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_HASH_HPP 

#include "tuple.hpp"

#include "function_traits.hpp"

#include <unordered_map>

#include <unordered_set>

#include <tuple>

#include <memory>

#include <cstring>

namespace cpp
{
    template <typename T>
    constexpr std::size_t hash(const T& value);
    
    template <typename T, typename U, typename ... Args>
    constexpr std::size_t hash(const T& first, const U& second, const Args&... tail);
    
    template <typename T>
    std::size_t raw_hash(const T& value);
    
    namespace 
    {
        template <typename T, cpp::FunctionKind Kind = cpp::function_kind<T>()>
        class HashDispatch;
        
        template <typename T>
        class HashDispatch<T, cpp::FunctionKind::FREE_FUNCTION>;
        
        template <typename T>
        class HashDispatch<T, cpp::FunctionKind::MEMBER_FUNCTION>;
        
        template <typename T>
        class HashDispatch<T, cpp::FunctionKind::CONST_MEMBER_FUNCTION>;
        
        template <typename T>
        class HashDispatch<T, cpp::FunctionKind::FUNCTOR>;
    }
    
    template <typename T>
    constexpr std::size_t hash_combine(std::size_t seed, const T& value);
    
    template <typename T, typename U>
    constexpr std::size_t hash(const std::pair<T, U>& pair);
    
    template <typename ... Ts>
    constexpr std::size_t hash(const std::tuple<Ts...>& tuple);
    
    template <typename T>
    struct Hash;
    
    template <typename T>
    class RawHash;
}
```

## Function template `cpp::hash<T, U, Args...>`<a id="cpp::hash<T, U, Args...>"></a>

``` cpp
template <typename T, typename U, typename ... Args>
constexpr std::size_t hash(const T& first, const U& second, const Args&... tail);
```

This function is a generalization of unary cpp::hash() that accepts two or more input values. The resulting hash code is computed as the hash combination (See cpp::hash\_combine()) of the first value and the hash combination of the rest:

``` haskell
hash first:second:tail -> hash_combine (hash first) (hash second:tail)
```

-----

## Function template `cpp::raw_hash<T>`<a id="cpp::raw_hash<T>"></a>

``` cpp
template <typename T>
std::size_t raw_hash(const T& value);
```

This function ignores the `std::hash` specialization of the value type and implements a bytewise hash value instead. Bytewise hash is computed as a hash combination of each byte of the value storage, in the range `[addressof(value), addressof(value) + sizeof(T))`. The value is copyed to an intermediary aligned storage to perform the byte traversal.

-----

## Function template `cpp::hash_combine<T>`<a id="cpp::hash_combine<T>"></a>

``` cpp
template <typename T>
constexpr std::size_t hash_combine(std::size_t seed, const T& value);
```

Literally copied from [this stack overflow thread](http://stackoverflow.com/questions/7110301/generic-hash-for-tuples-in-unordered-map-unordered-set) which in turn got it from [`boost::hash_combine()`](http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html).

### Parameter `cpp::hash_combine::seed`<a id="cpp::hash_combine::seed"></a>

``` cpp
std::size_t seed
```

Source hash value to combine. \\param value Value of type T wich has will be combined. \\return the combination between the seed value and the hash of the input value. See `boost::hash_combine()` link above for the specific combination function.

-----

## Class template `cpp::Hash<T>`<a id="cpp::Hash<T>"></a>

``` cpp
template <typename T>
struct Hash
{
    constexpr std::size_t operator()(const T& value) const;
};
```

This template provides the features of cpp::hash() as a functor template, suitable for unordered containers.

### Template parameter `cpp::Hash<T>::T`<a id="cpp::Hash<T>::T"></a>

``` cpp
typename T
```

Type of the values to get the hash code. See cpp::hash() for behavior and type requirements.

-----

## Class template `cpp::RawHash<T>`<a id="cpp::RawHash<T>"></a>

``` cpp
template <typename T>
class RawHash
{
public:
    constexpr std::size_t operator()(const T& value) const;
};
```

This template provides the features of cpp::raw\_hash() as a functor template, suitable for unordered containers.

### Template parameter `cpp::RawHash<T>::T`<a id="cpp::RawHash<T>::T"></a>

``` cpp
typename T
```

Type of the values to get the hash code. See cpp::raw\_hash() for behavior and type requirements.

-----
