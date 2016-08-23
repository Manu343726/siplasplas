#ifndef SIPLASPLAS_UTILITY_MEMORY_MANIP_HPP
#define SIPLASPLAS_UTILITY_MEMORY_MANIP_HPP

#include <memory>
#include <cstdint>
#include <climits>
#include <siplasplas/utility/export.hpp>

/**
 * \ingroup utility
 * \defgroup memory-manip
 * \brief Tools related to raw memory manipulation
 */

namespace cpp
{
    namespace detail
    {
        /**
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(char* pointer, std::size_t alignment);

        /**
         * \ingroup memory-manip
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(void* pointer, std::size_t alignment);

        /**
         * \ingroup memory-manip
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(const char* pointer, std::size_t alignment);

        /**
         * \ingroup memory-manip
         * \brief Checks if an address is aligned to a given boundary
         *
         * \param pointer Address to check
         * \param alignment Required alignment. Must be a power of two
         *
         * \returns true if \p pointer is aligned to \p alignment boundary. False otherwise.
         */
        SIPLASPLAS_UTILITY_EXPORT bool is_aligned(const void* pointer, std::size_t alignment);

        /**
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
         * \brief Tags a pointer with the specified data
         *
         * This function uses the tagged pointer technique to store data
         * in a 64 bit virtual memory address. Passing data of more than 16 bits wide has
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

            return reinterpret_cast<T*>(static_cast<std::uintptr_t>(data) << 48 | (reinterpret_cast<std::uintptr_t>(pointer) & 0x0000FFFFFFFFFFFF));
        }

        /**
         * \ingroup memory-manip
         * \brief Tags a pointer with the specified data
         *
         * This function uses the tagged pointer technique to store data
         * in a 64 bit virtual memory address. Passing data of more than 16 bits wide has
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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
         * \ingroup memory-manip
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

#ifndef SIPLASPLAS_UTILITY_ALIGNEDMALLOC_ALIGNOFFSET_BITS
        /**
         * \ingroup memory-manip
         * \brief Type used to store the offset between the aligned pointer
         * returned by aligned_malloc() and the beginning of the allocated block
         *
         * This type limits the maximum alignment requirement that can be passed to
         * aligned_malloc() stores the distance to the start of the allocated block
         * so it can be deallocated in aligned_free(). To use as little extra memory as possible,
         * a 8 bit unsigned integer is used by default, which means up to 256 byte alignment boundary
         * is supported by default. Users can change that limit by defining `SIPLASPLAS_UTILITY_ALIGNEDMALLOC_ALIGNOFFSET_BITS`
         * to the with in bits of the unsigned integer used for offset storage (8, 
         * 16, 32, and 64 are supported)
         *
         * ``` cpp
         * #define SIPLASPLAS_UTILITY_ALIGNEDMALLOC_ALIGNEDOFFSET_BITS 32
         * #include <siplasplas/utility/memory_manip.hpp>
         *
         * void* ptr =cpp::aligned_malloc(1024, 1024); // Allocate 1024 bytes in the 1024 boundary
         * ```
         */
        using AlignedMallocAlingOffset = std::uint8_t;
#else
        using AlignedMallocAlingOffset = SIPLASPLAS_PP_CAT(
            SIPLASPLAS_PP_CAT(std::uint,SIPLASPLAS_UTILITY_ALIGNEDMALLOC_ALIGNOFFSET_BITS),
            _t
        );
#endif

        /**
         * \ingroup memory-manip
         * \brief Allocates a block of memory of memory aligned to an specific boundary
         *
         * This function allocates a memory block starting at a specific alignment boundary.
         * Users can also set some extra bytes for bookeeping data before the returned block.
         * To deallocate blocks allocated with aligned_malloc(), use aligned_free(), never std::free()
         *
         * \param size Requested block size in bytes
         * \param alignment Required alignment boundary. Must be a power of two
         * \param offset Extra space reserved for the user right brefore the returned
         * block. 0 by default.
         *
         * \returns A pointer to a memory block of \p size bytes, multiple of \p alignment. nullptr
         * if fails
         */
        SIPLASPLAS_UTILITY_EXPORT void* aligned_malloc(std::size_t size, std::size_t alignment, std::size_t offset = 0);

        /**
         * \ingroup memory-manip
         * \brief Returns a pointer to the full block allocated by cpp::aligned_malloc()
         *
         * aligned_malloc() allocates an oversized block in order to acomplish the alignment requirements
         * While aligned_malloc() returns the expected aligned block, this function can be used to get the
         * complete allocated block.
         *
         * \param pointer Pointer to a block allocated by cpp::aligned_malloc()
         * \param offset User offset. The behavior is undefined if it's different
         * to the argument given to cpp::aligned_malloc() when allocating the block
         *
         * \returns A pointer to the beginning of the complete allocated block. This pointer can be
         * deallocated by `std::free()`.
         */
        SIPLASPLAS_UTILITY_EXPORT void* aligned_malloc_block(void* pointer, std::size_t offset = 0);

        /**
         * \ingroup memory-manip
         * \brief Deallocates a block allocated by cpp::aligned_malloc()
         *
         * \param pointer Pointer to the allocated block
         * \param offset User offset. The behavior is undefined if it's different
         * to the argument given to cpp::aligned_malloc() when allocating the block
         */
        SIPLASPLAS_UTILITY_EXPORT void aligned_free(void* pointer, std::size_t offset = 0);

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

