#ifndef SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP
#define SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

#include <memory>
#include <cassert>

namespace cpp
{
    namespace detail
    {
        char* aligned_ptr(char* pointer, std::size_t alignment)
        {
            char* result = pointer + alignment - (reinterpret_cast<std::uintptr_t>(pointer) % alignment);
			assert(result >= pointer && "Aligned pointer should be greater");
			return result;
		}

        void* aligned_ptr(void* pointer, std::size_t alignment)
        {
            return aligned_ptr(reinterpret_cast<char*>(pointer), alignment);
        }

        template<typename T>
        void write_at(char* pointer, const T& value, std::intptr_t offset = 0)
        {
            *(reinterpret_cast<T*>(pointer) + offset) = value;
        }

        template<typename T>
        void write_at(void* pointer, const T& value, std::intptr_t offset = 0)
        {
            write_at(reinterpret_cast<char*>(pointer), value, offset);
        }

        template<typename T>
        T read_at(char* pointer, std::intptr_t offset = 0)
        {
            return *(reinterpret_cast<T*>(pointer + offset));
        }

        template<typename T>
        T read_at(void* pointer, std::intptr_t offset = 0)
        {
            return read_at<T>(reinterpret_cast<char*>(pointer), offset);
        }

        template<typename T>
        void write_before(char* pointer, const T& value)
        {
            write_at(pointer, value, -sizeof(T));
        }

        template<typename T>
        void write_before(void* pointer, const T& value)
        {
            write_before(reinterpret_cast<char*>(pointer), value);
        }

        template<typename T>
        T read_before(char* pointer)
        {
            return read_at<T>(pointer, - sizeof(T));
        }

        template<typename T>
        T read_before(void* pointer)
        {
            return read_before<T>(reinterpret_cast<char*>(pointer));
        }
    }
}

#endif // SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

