#ifndef SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP
#define SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

#include <memory>

namespace cpp
{
    namespace detail
    {
        char* aligned_ptr(char* pointer, std::size_t alignment)
        {
            return pointer + (reinterpret_cast<std::uintptr_t>(pointer) % alignment);
        }

        template<typename T>
        void write_at(char* pointer, const T& value)
        {
            *reinterpret_cast<T*>(pointer) = value;
        }

        template<typename T>
        T read_at(char* pointer)
        {
            return *reinterpret_cast<T*>(pointer);
        }

        template<typename T>
        void write_before(char* pointer, const T& value)
        {
            write_at(pointer - sizeof(T), value);
        }

        template<typename T>
        T read_before(char* pointer)
        {
            return read_at<T>(pointer - sizeof(T));
        }
    }
}

#endif // SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

