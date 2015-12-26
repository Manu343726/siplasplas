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

        void* aligned_ptr(void* pointer, std::size_t alignment)
        {
            return aligned_ptr(reinterpret_cast<char*>(pointer), alignment);
        }

        template<typename T>
        void write_at(char* pointer, const T& value)
        {
            *reinterpret_cast<T*>(pointer) = value;
        }

        template<typename T>
        void write_at(void* pointer, const T& value)
        {
            write_at(reinterpret_cast<char*>(pointer), value);
        }

        template<typename T>
        T read_at(char* pointer)
        {
            return *reinterpret_cast<T*>(pointer);
        }

        template<typename T>
        T read_at(void* pointer)
        {
            return read_at<T>(reinterpret_cast<char*>(pointer));
        }

        template<typename T>
        void write_before(char* pointer, const T& value)
        {
            write_at(pointer - sizeof(T), value);
        }

        template<typename T>
        void write_before(void* pointer, const T& value)
        {
            write_before(reinterpret_cast<char*>(pointer), value);
        }

        template<typename T>
        T read_before(char* pointer)
        {
            return read_at<T>(pointer - sizeof(T));
        }

        template<typename T>
        T read_before(void* pointer)
        {
            return read_before<T>(reinterpret_cast<char*>(pointer));
        }
    }
}

#endif // SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

