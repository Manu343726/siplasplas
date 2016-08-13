#ifndef SIPLASPLAS_TYPEERASURE_ANYSTORAGE_FIXEDSIZE_HPP
#define SIPLASPLAS_TYPEERASURE_ANYSTORAGE_FIXEDSIZE_HPP

#include <siplasplas/utility/memory_manip.hpp>
#include <array>
#include <type_traits>

namespace cpp
{

/**
 * \ingroup any storage
 * \brief Implements fixed-size storage
 *
 * \tparam Size Max capacity of the storage
 * \tparam Alignment Storage alignment. `Alignof(std::uint8_t`) by default
 */
template<std::size_t Size, std::size_t Alignment = alignof(std::uint8_t)>
class FixedSizeStorage
{
protected:
    /**
     * \brief Returns a pointer to the beginning of the storage
     */
    const char* begin() const
    {
        return reinterpret_cast<const char*>(&_storage);
    }

    /**
     * \brief Returns a pointer to the beginning of the storage
     */
    char* begin()
    {
        return reinterpret_cast<char*>(&_storage);
    }

    /**
     * \brief Returns a pointer to the end of the storage
     */
    const char* end() const
    {
        return begin() + size();
    }

    /**
     * \brief Returns a pointer to the end of the storage
     */
    char* end()
    {
        return begin() + size();
    }

    /**
     * \brief Returns the size in bytes of the storage
     */
    constexpr std::size_t size() const
    {
        return Size;
    }

    /**
     * \brief Returns a pointer to the storage memory space
     *
     * \param alignment Required storage alignment. Byte boundary by default
     *
     * \returns An aligned pointer to the storage. The behavior is undefined if
     * the storage has no space to host an object with the required alignment
     * (See objectFitsInStorage()).
     */
    const void* storage(std::size_t alignment = alignof(std::uint8_t)) const
    {
        return cpp::detail::aligned_ptr(begin(), alignment);
    }

    /**
     * \brief Returns a pointer to the storage memory space
     *
     * \param alignment Required storage alignment. Byte boundary by default
     *
     * \returns An aligned pointer to the storage. The behavior is undefined if
     * the storage has no space to host an object with the required alignment
     * (See objectFitsInStorage()).
     */
    void* storage(std::size_t alignment = alignof(std::uint8_t))
    {
        return cpp::detail::aligned_ptr(begin(), alignment);
    }

    /**
     * \brief Checks if an object can be hosted in the storage space
     * \returns True if properly-aligned values of type T fit in the storage,
     * false otherwise
     */
    template<typename T>
    bool objectFitsInStorage() const
    {
        return reinterpret_cast<const char*>(storage(alignof(T))) + sizeof(T) <= end();
    }

private:
    std::aligned_storage_t<Size, Alignment> _storage;
};

}

#endif // SIPLASPLAS_TYPEERASURE_ANYSTORAGE_FIXEDSIZE_HPP
