#ifndef SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP
#define SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

#include <memory>
#include <cassert>

namespace cpp
{
    namespace detail
    {
        char* aligned_ptr(char* pointer, std::size_t alignment);
        void* aligned_ptr(void* pointer, std::size_t alignment);

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
        T read_at(const char* pointer, std::intptr_t offset = 0)
        {
            return *(reinterpret_cast<const T*>(pointer + offset));
        }

        template<typename T>
        T read_at(const void* pointer, std::intptr_t offset = 0)
        {
            return read_at<T>(reinterpret_cast<const char*>(pointer), offset);
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
        T read_before(const char* pointer)
        {
            return read_at<T>(pointer, - sizeof(T));
        }

        template<typename T>
        T read_before(const void* pointer)
        {
            return read_before<T>(reinterpret_cast<const char*>(pointer));
        }

        template<typename T>
        class RawReaderWriter
        {
        public:
            RawReaderWriter(void* at) :
                _at{reinterpret_cast<char*>(at)}
            {}

            T get() const
            {
                return detail::read_at<T>(_at);
            }

            operator T() const
            {
                return get();
            }

            T operator=(T value)
            {
                detail::write_at(_at, value);
                return value;
            }
        private:
            char* _at;
        };
    }
}

#endif // SIPLASPLAS_ALLOCATOR_ALLOCATOR_UTILS_HPP

