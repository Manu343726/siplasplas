#ifndef SIPLASPLAS_TYPEERASURE_ANYSTORAGE_DEADPOOL_HPP
#define SIPLASPLAS_TYPEERASURE_ANYSTORAGE_DEADPOOL_HPP

#include <siplasplas/utility/memory_manip.hpp>
#include <siplasplas/typeerasure/typeinfo.hpp>
#include <type_traits>

namespace cpp
{

/**
 * \ingroup any storage
 * \brief Implements fixed-size with fallback to dynamic allocation
 *
 * DeadPoolStorage implements a fixed-size storage similar to FixedSizeStorage
 * but that switches to dynamic allocation if there's no enough space in the
 * fixed-size storage to allocate the object
 *
 * Dynamic allocation is performed through cpp::detail::aligned_malloc()
 *
 * \tparam PreAllocatedSize Max capacity of the storage
 * \tparam PreAllocatedAlignment Storage alignment. `Alignof(std::uint8_t`) by default
 */
template<std::size_t PreallocatedSize, std::size_t PreallocatedAlignment = alignof(std::uint8_t)>
class DeadPoolStorage
{
public:
    static_assert(PreallocatedSize >= sizeof(void*),
        "At least sizeof(void*) bytes of preallocated storage is needed");

    void* storage(cpp::typeerasure::TypeInfo typeInfo) const
    {
        if(dynamicAllocStorage() &&
           _dynamicAllocStorageSize == typeInfo.sizeOf() &&
           cpp::detail::is_aligned(dynamicAllocStoragePointer(), typeInfo.alignment()))
        {
            return dynamicAllocStoragePointer();
        }
        else
        {
            if(dynamicAllocStorage())
            {
                cpp::detail::aligned_free(dynamicAllocStoragePointer());
            }

            char* storagePtr = reinterpret_cast<char*>(&_preallocatedStorage);
            char* alignedPointerToFixedStorage = cpp::detail::aligned_ptr(
                storagePtr,
                typeInfo.alignment()
            );

            if(alignedPointerToFixedStorage + typeInfo.sizeOf() <= storagePtr + PreallocatedSize)
            {
                _dynamicAllocStorageSize = 0;
                return alignedPointerToFixedStorage;
            }
            else
            {
                dynamicAllocStoragePointer(
                    cpp::detail::aligned_malloc(typeInfo.sizeOf(), typeInfo.alignment())
                );

                _dynamicAllocStorageSize = typeInfo.sizeOf();

                return dynamicAllocStoragePointer();
            }
        }
    }

    template<typename T>
    constexpr bool objectFitsInStorage() const
    {
        return true;
    }

    ~DeadPoolStorage()
    {
        if(dynamicAllocStorage())
        {
            cpp::detail::aligned_free(dynamicAllocStoragePointer());
        }
    }

    std::size_t dynamicAllocStorageSize() const
    {
        return _dynamicAllocStorageSize;
    }

private:
    mutable std::size_t _dynamicAllocStorageSize = 0;
    mutable std::aligned_storage_t<PreallocatedSize, PreallocatedAlignment> _preallocatedStorage;

    bool dynamicAllocStorage() const
    {
        return _dynamicAllocStorageSize > 0;
    }

    void* dynamicAllocStoragePointer() const
    {
        return cpp::detail::read_at<void*>(&_preallocatedStorage);
    }

    void dynamicAllocStoragePointer(void* pointer) const
    {
        cpp::detail::write_at(&_preallocatedStorage, pointer);
    }
};

}

#endif // SIPLASPLAS_TYPEERASURE_ANYSTORAGE_DEADPOOL_HPP
