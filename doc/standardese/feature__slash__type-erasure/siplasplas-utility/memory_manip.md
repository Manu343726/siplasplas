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
