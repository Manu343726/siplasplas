---
layout: standardese-doc
---

# Header file `destroy.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_DESTROY_HPP 

#include <type_traits>

namespace cpp
{
    template <typename T>
    void destroy(T& object);
    
    template <typename T>
    void destroy(T* objectPtr);
    
    template <typename T, typename ... Args>
    void construct(void* where, Args&&... args);
    
    template <typename T, typename ... Args>
    void construct(T* where, Args&&... args);
}
```

## Function template `cpp::construct<T, Args...>`<a id="cpp::construct<T, Args...>"></a>

``` cpp
template <typename T, typename ... Args>
void construct(void* where, Args&&... args);
```

This function performs an in-place construction of an object of type T in the given address. Arguments are passed as-is to the object constructor. The behavior is undefined if `alignment(pointer) != alignof(T)`.

``` cpp
std::aligned_storage<std::string> storage;
cpp::construct<std::string>(&storage, "hello, world!");
```

### Parameter `cpp::construct::where`<a id="cpp::construct::where"></a>

``` cpp
void* where
```

Location of the object \\param args Constructor arguments

-----

## Function template `cpp::construct<T, Args...>`<a id="cpp::construct<T, Args...>"></a>

``` cpp
template <typename T, typename ... Args>
void construct(T* where, Args&&... args);
```

This function performs an in-place construction of an object of type T in the given address. Arguments are passed as-is to the object constructor. The behavior is undefined if `alignment(pointer) != alignof(T)`.

``` cpp
std::aligned_storage<std::string> storage;
std::string* stringPtr = reinterpret_cast<std::string*>(&storage);

cpp::construct(stringPtr, "hello, world!");
```

### Parameter `cpp::construct::where`<a id="cpp::construct::where"></a>

``` cpp
T* where
```

Location of the object \\param args Constructor arguments

-----
