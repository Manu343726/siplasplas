---
layout: standardese-doc
---

# Header file `memory_manip.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_MEMORY_MANIP_HPP 

#include <memory>

#include <siplasplas/utility/export.hpp>

namespace cpp
{
    namespace detail
    {
         char* aligned_ptr(char* pointer, std::size_t alignment);
        
         void* aligned_ptr(void* pointer, std::size_t alignment);
        
         bool is_aligned(char* pointer, std::size_t alignment);
        
         bool is_aligned(void* pointer, std::size_t alignment);
        
        template <typename T>
        void write_at(char* pointer, const T& value, std::intptr_t offset = 0);
        
        template <typename T>
        void write_at(void* pointer, const T& value, std::intptr_t offset = 0);
        
        template <typename T>
        T read_at(const char* pointer, std::intptr_t offset = 0);
        
        template <typename T>
        T read_at(const void* pointer, std::intptr_t offset = 0);
        
        template <typename T>
        void write_before(char* pointer, const T& value);
        
        template <typename T>
        void write_before(void* pointer, const T& value);
        
        template <typename T>
        T read_before(const char* pointer);
        
        template <typename T>
        T read_before(const void* pointer);
        
        template <typename T>
        class RawReaderWriter;
    }
}
```

## Function `cpp::detail::aligned_ptr`<a id="cpp::detail::aligned_ptr"></a>

``` cpp
 char* aligned_ptr(char* pointer, std::size_t alignment);
```

*Returns:* The next (upper) address from the given address that's aligned to the required boundary

### Parameter `cpp::detail::aligned_ptr::pointer`<a id="cpp::detail::aligned_ptr::pointer"></a>

``` cpp
char* pointer
```

Starting address \\param alignment Alignment boundary. Must be a power of two

-----

## Function `cpp::detail::aligned_ptr`<a id="cpp::detail::aligned_ptr"></a>

``` cpp
 void* aligned_ptr(void* pointer, std::size_t alignment);
```

*Returns:* The next (upper) address from the given address that's aligned to the required boundary

### Parameter `cpp::detail::aligned_ptr::pointer`<a id="cpp::detail::aligned_ptr::pointer"></a>

``` cpp
void* pointer
```

Starting address \\param alignment Alignment boundary. Must be a power of two

-----

## Function `cpp::detail::is_aligned`<a id="cpp::detail::is_aligned"></a>

``` cpp
 bool is_aligned(char* pointer, std::size_t alignment);
```

*Returns:* true if \\p pointer is aligned to \\p alignment boundary. False otherwise.

### Parameter `cpp::detail::is_aligned::pointer`<a id="cpp::detail::is_aligned::pointer"></a>

``` cpp
char* pointer
```

Address to check \\param alignment Required alignment. Must be a power of two

-----

## Function `cpp::detail::is_aligned`<a id="cpp::detail::is_aligned"></a>

``` cpp
 bool is_aligned(void* pointer, std::size_t alignment);
```

*Returns:* true if \\p pointer is aligned to \\p alignment boundary. False otherwise.

### Parameter `cpp::detail::is_aligned::pointer`<a id="cpp::detail::is_aligned::pointer"></a>

``` cpp
void* pointer
```

Address to check \\param alignment Required alignment. Must be a power of two

-----
