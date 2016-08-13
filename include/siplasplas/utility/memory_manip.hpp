#ifndef SIPLASPLAS_UTILITY_MEMORY_MANIP_HPP
#define SIPLASPLAS_UTILITY_MEMORY_MANIP_HPP

#include <memory>
#include <cstdint>
#include <climits>
#include <siplasplas/utility/export.hpp>


namespace cpp
{
    namespace detail
    {
        /**
         * \ingroup utility
         * \brief Returns an address aligned to an specific boundary
         *
         * \param pointer Starting address
         * \param alignment Alignment boundary. Must be a power of two
         *
         * \returns The next (upper) address from the given address that's aligned
         * to the required boundary
         */
        SIPLASPLAS_UTILITY_EXPORT char* aligned_ptr(char* pointer, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Returns an address aligned to an specific boundary
         *
         * \param pointer Starting address
         * \param alignment Alignment boundary. Must be a power of two
         *
         * \returns The next (upper) address from the given address that's aligned
         * to the required boundary
         */
        SIPLASPLAS_UTILITY_EXPORT void* aligned_ptr(void* pointer, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Returns an address aligned to an specific boundary
         *
         * \param pointer Starting address
         * \param alignment Alignment boundary. Must be a power of two
         *
         * \returns The next (upper) address from the given address that's aligned
         * to the required boundary
         */
        SIPLASPLAS_UTILITY_EXPORT const char* aligned_ptr(const char* pointer, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Returns an address aligned to an specific boundary
         *
         * \param pointer Starting address
         * \param alignment Alignment boundary. Must be a power of two
         *
         * \returns The next (upper) address from the given address that's aligned
         * to the required boundary
         */
        SIPLASPLAS_UTILITY_EXPORT const void* aligned_ptr(const void* pointer, std::size_t alignment);


        /**
         * \ingroup utility
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(char* pointer, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(void* pointer, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(const char* pointer, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(const void* pointer, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Returns the distance between a memory address and the next address aligned to the given
         * boundary
         *
         * \param address Memory address
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns The distance in bytes to the next aligned address
         */
        SIPLASPLAS_UTILITY_EXPORT std::uintptr_t missalignment(const char* address, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Returns the distance between a memory address and the next address aligned to the given
         * boundary
         *
         * \param address Memory address
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns The distance in bytes to the next aligned address
         */
        SIPLASPLAS_UTILITY_EXPORT std::uintptr_t missalignment(const void* address, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Returns the distance between a memory address and the next address aligned to the given
         * boundary
         *
         * \param address Memory address
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns The distance in bytes to the next aligned address
         */
        SIPLASPLAS_UTILITY_EXPORT std::uintptr_t missalignment(char* address, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Returns the distance between a memory address and the next address aligned to the given
         * boundary
         *
         * \param address Memory address
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns The distance in bytes to the next aligned address
         */
        SIPLASPLAS_UTILITY_EXPORT std::uintptr_t missalignment(void* address, std::size_t alignment);

        /**
         * \ingroup utility
         * \brief Tags a pointer with the specified data
         *
         * This function uses the tagged pointer technique to store data
         * in a 64 bit virtual memory address. Passing data of more that 16 bits wide has
         * undefined behavior. Compilation fails if this function is used in non 64 bit architectures.
         * Note accessing a tagged pointer directly may cause a segmentation fault. See cpp::untagPointer().
         *
         * \tparam T Pointed type
         * \tparam U Must be an integral type of max sizeof(U) = 16 bit
         * \param pointer pointer where to store data
         * \param data data to store
         *
         * \returns A pointer of type T* with the data and the same address
         */
        template<typename T, typename U>
        T* tagPointer(T* pointer, U data)
        {
            static_assert(sizeof(nullptr) * CHAR_BIT == 64, "Tagging pointers is only supported in architectures with 64 bit virtual addresses");

            return reinterpret_cast<T*>(data << 48 | (reinterpret_cast<std::uintptr_t>(pointer) & 0x0000FFFFFFFFFFFF));
        }

        /**
         * \ingroup utility
         * \brief Tags a pointer with the specified data
         *
         * This function uses the tagged pointer technique to store data
         * in a 64 bit virtual memory address. Passing data of more that 16 bits wide has
         * undefined behavior. Compilation fails if this function is used in non 64 bit architectures.
         * Note accessing a tagged pointer directly may cause a segmentation fault. See cpp::untagPointer().
         *
         * \tparam U Must be an integral type of max sizeof(U) = 16 bit
         * \param pointer pointer where to store data
         * \param data data to store
         *
         * \returns A pointer of type T* with the data and the same address
         */
        template<typename R, typename U, typename... Args>
        auto tagPointer(R(*pointer)(Args...), U data) -> decltype(pointer)
        {
            static_assert(sizeof(nullptr) * CHAR_BIT == 64, "Tagging pointers is only supported in architectures with 64 bit virtual addresses");

            return reinterpret_cast<R(*)(Args...)>(data << 48 | (reinterpret_cast<std::uintptr_t>(pointer) & 0x0000FFFFFFFFFFFF));
        }

        /**
         * \ingroup utility
         * \brief Untags a pointer
         *
         * Assuming the pointer is a tagged pointer, this function removes the tagged data
         * and returns the memory address ready to be referenced.
         * Compilation fails if this function is used in non 64 bit architectures.
         *
         * \param pointer Tagged pointer to clear
         *
         * \returns The given pointer with the 16 upper bits cleared
         */
        template<typename T>
        T* untagPointer(T* pointer)
        {
            static_assert(sizeof(nullptr) * CHAR_BIT == 64, "Tagging pointers is only supported in architectures with 64 bit virtual addresses");

            return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(pointer) & 0x0000FFFFFFFFFFFF);
        }

        /**
         * \ingroup utility
         * \brief Untags a pointer
         *
         * Assuming the pointer is a tagged pointer, this function removes the tagged data
         * and returns the memory address ready to be referenced.
         * Compilation fails if this function is used in non 64 bit architectures.
         *
         * \param pointer Tagged pointer to clear
         *
         * \returns The given pointer with the 16 upper bits cleared
         */
        template<typename R, typename... Args>
        auto untagPointer(R(*pointer)(Args...)) -> decltype(pointer)
        {
            static_assert(sizeof(nullptr) * CHAR_BIT == 64, "Tagging pointers is only supported in architectures with 64 bit virtual addresses");

            return reinterpret_cast<R(*)(Args...)>(reinterpret_cast<std::uintptr_t>(pointer) & 0x0000FFFFFFFFFFFF);
        }

        /**
         * \ingroup utility
         * \brief Reads the data stored in a tagged pointer
         *
         * Assuming the pointer is a tagged pointer, this function reads the data
         * tagged in the 16 more significative bits of the pointer.
         * Compilation fails if this function is used in non 64 bit architectures.
         *
         * \param pointer Tagged pointer to read
         *
         * \returns The 16 bit unsigned integer value tagged in the pointer
         */
        template<typename T>
        std::uint16_t readTaggedPointer(T* pointer)
        {
            static_assert(sizeof(nullptr) * CHAR_BIT == 64, "Tagging pointers is only supported in architectures with 64 bit virtual addresses");

            return reinterpret_cast<std::uintptr_t>(pointer) >> 48;
        }

        /**
         * \ingroup utility
         * \brief Reads the data stored in a tagged pointer
         *
         * Assuming the pointer is a tagged pointer, this function reads the data
         * tagged in the 16 more significative bits of the pointer.
         * Compilation fails if this function is used in non 64 bit architectures.
         *
         * \param pointer Tagged pointer to read
         *
         * \returns The 16 bit unsigned integer value tagged in the pointer
         */
        template<typename R, typename... Args>
        std::uint16_t readTaggedPointer(R(*pointer)(Args...))
        {
            static_assert(sizeof(nullptr) * CHAR_BIT == 64, "Tagging pointers is only supported in architectures with 64 bit virtual addresses");

            return reinterpret_cast<std::uintptr_t>(pointer) >> 48;
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

#endif // SIPLASPLAS_UTILITY_MEMORY_MANIP_HPP

