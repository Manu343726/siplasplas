---
layout: standardese-doc
---

# Header file `memory_manip.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_MEMORY_MANIP_HPP 

#include <memory>

#include <cstdint>

#include <climits>

#include <siplasplas/utility/export.hpp>

namespace cpp
{
    namespace detail
    {
         char* aligned_ptr(char* pointer, std::size_t alignment);
        
         void* aligned_ptr(void* pointer, std::size_t alignment);
        
         const char* aligned_ptr(const char* pointer, std::size_t alignment);
        
         const void* aligned_ptr(const void* pointer, std::size_t alignment);
        
         bool is_aligned(char* pointer, std::size_t alignment);
        
         bool is_aligned(void* pointer, std::size_t alignment);
        
         bool is_aligned(const char* pointer, std::size_t alignment);
        
         bool is_aligned(const void* pointer, std::size_t alignment);
        
         std::uintptr_t missalignment(const char* address, std::size_t alignment);
        
         std::uintptr_t missalignment(const void* address, std::size_t alignment);
        
         std::uintptr_t missalignment(char* address, std::size_t alignment);
        
         std::uintptr_t missalignment(void* address, std::size_t alignment);
        
        template <typename T, typename U>
        T* tagPointer(T* pointer, U data);
        
        template <typename R, typename U, typename ... Args>
        decltype(pointer) tagPointer(R(*pointer)(Args...), U data);
        
        template <typename T>
        T* untagPointer(T* pointer);
        
        template <typename R, typename ... Args>
        decltype(pointer) untagPointer(R(*pointer)(Args...));
        
        template <typename T>
        std::uint16_t readTaggedPointer(T* pointer);
        
        template <typename R, typename ... Args>
        std::uint16_t readTaggedPointer(R(*pointer)(Args...));
        
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

## Function `cpp::detail::aligned_ptr`<a id="cpp::detail::aligned_ptr"></a>

``` cpp
 const char* aligned_ptr(const char* pointer, std::size_t alignment);
```

*Returns:* The next (upper) address from the given address that's aligned to the required boundary

### Parameter `cpp::detail::aligned_ptr::pointer`<a id="cpp::detail::aligned_ptr::pointer"></a>

``` cpp
const char* pointer
```

Starting address \\param alignment Alignment boundary. Must be a power of two

-----

## Function `cpp::detail::aligned_ptr`<a id="cpp::detail::aligned_ptr"></a>

``` cpp
 const void* aligned_ptr(const void* pointer, std::size_t alignment);
```

*Returns:* The next (upper) address from the given address that's aligned to the required boundary

### Parameter `cpp::detail::aligned_ptr::pointer`<a id="cpp::detail::aligned_ptr::pointer"></a>

``` cpp
const void* pointer
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

## Function `cpp::detail::is_aligned`<a id="cpp::detail::is_aligned"></a>

``` cpp
 bool is_aligned(const char* pointer, std::size_t alignment);
```

*Returns:* true if \\p pointer is aligned to \\p alignment boundary. False otherwise.

### Parameter `cpp::detail::is_aligned::pointer`<a id="cpp::detail::is_aligned::pointer"></a>

``` cpp
const char* pointer
```

Address to check \\param alignment Required alignment. Must be a power of two

-----

## Function `cpp::detail::is_aligned`<a id="cpp::detail::is_aligned"></a>

``` cpp
 bool is_aligned(const void* pointer, std::size_t alignment);
```

*Returns:* true if \\p pointer is aligned to \\p alignment boundary. False otherwise.

### Parameter `cpp::detail::is_aligned::pointer`<a id="cpp::detail::is_aligned::pointer"></a>

``` cpp
const void* pointer
```

Address to check \\param alignment Required alignment. Must be a power of two

-----

## Function `cpp::detail::missalignment`<a id="cpp::detail::missalignment"></a>

``` cpp
 std::uintptr_t missalignment(const char* address, std::size_t alignment);
```

*Returns:* The distance in bytes to the next aligned address

### Parameter `cpp::detail::missalignment::address`<a id="cpp::detail::missalignment::address"></a>

``` cpp
const char* address
```

Memory address \\param alignment Required alignment. Must be a power of two

-----

## Function `cpp::detail::missalignment`<a id="cpp::detail::missalignment"></a>

``` cpp
 std::uintptr_t missalignment(const void* address, std::size_t alignment);
```

*Returns:* The distance in bytes to the next aligned address

### Parameter `cpp::detail::missalignment::address`<a id="cpp::detail::missalignment::address"></a>

``` cpp
const void* address
```

Memory address \\param alignment Required alignment. Must be a power of two

-----

## Function `cpp::detail::missalignment`<a id="cpp::detail::missalignment"></a>

``` cpp
 std::uintptr_t missalignment(char* address, std::size_t alignment);
```

*Returns:* The distance in bytes to the next aligned address

### Parameter `cpp::detail::missalignment::address`<a id="cpp::detail::missalignment::address"></a>

``` cpp
char* address
```

Memory address \\param alignment Required alignment. Must be a power of two

-----

## Function `cpp::detail::missalignment`<a id="cpp::detail::missalignment"></a>

``` cpp
 std::uintptr_t missalignment(void* address, std::size_t alignment);
```

*Returns:* The distance in bytes to the next aligned address

### Parameter `cpp::detail::missalignment::address`<a id="cpp::detail::missalignment::address"></a>

``` cpp
void* address
```

Memory address \\param alignment Required alignment. Must be a power of two

-----

## Function template `cpp::detail::tagPointer<T, U>`<a id="cpp::detail::tagPointer<T, U>"></a>

``` cpp
template <typename T, typename U>
T* tagPointer(T* pointer, U data);
```

This function uses the tagged pointer technique to store data in a 64 bit virtual memory address. Passing data of more that 16 bits wide has undefined behavior. Compilation fails if this function is used in non 64 bit architectures. Note accessing a tagged pointer directly may cause a segmentation fault. See cpp::untagPointer().

*Returns:* A pointer of type T\* with the data and the same address

### Template parameter `cpp::detail::tagPointer<T, U>::T`<a id="cpp::detail::tagPointer<T, U>::T"></a>

``` cpp
typename T
```

Pointed type \\tparam U Must be an integral type of max sizeof(U) = 16 bit \\param pointer pointer where to store data \\param data data to store

-----

## Function template `cpp::detail::tagPointer<R, U, Args...>`<a id="cpp::detail::tagPointer<R, U, Args...>"></a>

``` cpp
template <typename R, typename U, typename ... Args>
decltype(pointer) tagPointer(R(*pointer)(Args...), U data);
```

This function uses the tagged pointer technique to store data in a 64 bit virtual memory address. Passing data of more that 16 bits wide has undefined behavior. Compilation fails if this function is used in non 64 bit architectures. Note accessing a tagged pointer directly may cause a segmentation fault. See cpp::untagPointer().

*Returns:* A pointer of type T\* with the data and the same address

### Template parameter `cpp::detail::tagPointer<R, U, Args...>::U`<a id="cpp::detail::tagPointer<R, U, Args...>::U"></a>

``` cpp
typename U
```

Must be an integral type of max sizeof(U) = 16 bit \\param pointer pointer where to store data \\param data data to store

-----

## Function template `cpp::detail::untagPointer<T>`<a id="cpp::detail::untagPointer<T>"></a>

``` cpp
template <typename T>
T* untagPointer(T* pointer);
```

Assuming the pointer is a tagged pointer, this function removes the tagged data and returns the memory address ready to be referenced. Compilation fails if this function is used in non 64 bit architectures.

*Returns:* The given pointer with the 16 upper bits cleared

### Parameter `cpp::detail::untagPointer::pointer`<a id="cpp::detail::untagPointer::pointer"></a>

``` cpp
T* pointer
```

Tagged pointer to clear

-----

## Function template `cpp::detail::untagPointer<R, Args...>`<a id="cpp::detail::untagPointer<R, Args...>"></a>

``` cpp
template <typename R, typename ... Args>
decltype(pointer) untagPointer(R(*pointer)(Args...));
```

Assuming the pointer is a tagged pointer, this function removes the tagged data and returns the memory address ready to be referenced. Compilation fails if this function is used in non 64 bit architectures.

*Returns:* The given pointer with the 16 upper bits cleared

### Parameter `cpp::detail::untagPointer::pointer`<a id="cpp::detail::untagPointer::pointer"></a>

``` cpp
R(*pointer)(Args...)
```

Tagged pointer to clear

-----

## Function template `cpp::detail::readTaggedPointer<T>`<a id="cpp::detail::readTaggedPointer<T>"></a>

``` cpp
template <typename T>
std::uint16_t readTaggedPointer(T* pointer);
```

Assuming the pointer is a tagged pointer, this function reads the data tagged in the 16 more significative bits of the pointer. Compilation fails if this function is used in non 64 bit architectures.

*Returns:* The 16 bit unsigned integer value tagged in the pointer

### Parameter `cpp::detail::readTaggedPointer::pointer`<a id="cpp::detail::readTaggedPointer::pointer"></a>

``` cpp
T* pointer
```

Tagged pointer to read

-----

## Function template `cpp::detail::readTaggedPointer<R, Args...>`<a id="cpp::detail::readTaggedPointer<R, Args...>"></a>

``` cpp
template <typename R, typename ... Args>
std::uint16_t readTaggedPointer(R(*pointer)(Args...));
```

Assuming the pointer is a tagged pointer, this function reads the data tagged in the 16 more significative bits of the pointer. Compilation fails if this function is used in non 64 bit architectures.

*Returns:* The 16 bit unsigned integer value tagged in the pointer

### Parameter `cpp::detail::readTaggedPointer::pointer`<a id="cpp::detail::readTaggedPointer::pointer"></a>

``` cpp
R(*pointer)(Args...)
```

Tagged pointer to read

-----
