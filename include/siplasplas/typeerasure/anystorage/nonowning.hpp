#ifndef SIPLASPLAS_TYPEERASURE_ANYSTORAGE_NONOWNING_HPP
#define SIPLASPLAS_TYPEERASURE_ANYSTORAGE_NONOWNING_HPP

#include <siplasplas/typeerasure/typeinfo.hpp>

namespace cpp
{

/**
 * \ingroup any storage
 * \brief Implements a read-only non owning storage
 *
 * A non owning storage stores a reference to an existing
 * object instead of hosting it. Size of non owning storages
 * is always `sizeof(void*)`.
 */
class ConstNonOwningStorage
{
public:
    /**
     * \brief Initializes the storage pointing it to the
     * given object
     *
     * \param object Pointer to the object to reference
     */
    ConstNonOwningStorage(const void* object) :
        _reference{object}
    {}

    /**
     * \brief Reassigns the storage to reference another
     * object
     *
     * \param object Pointer to the new object to reference
     */
    void rebind(const void* object)
    {
        _reference = object;
    }

    /**
     * \brief Returns a pointer to the storage memory space
     *
     * \param typeInfo Type information with the required alignment and storage size
     *
     * \returns A pointer to the reference object. Alignment is ignored, this
     * function assumes the pointer to the referenced object is valid
     */
    const void* storage(cpp::typeerasure::TypeInfo typeInfo) const
    {
        return _reference;
    }

private:
    const void* _reference;
};

/**
 * \ingroup any storage
 * \brief Implements a non owning storage
 *
 * A non owning storage stores a reference to an existing
 * object instead of hosting it. Size of non owning storages
 * is always `sizeof(void*)`.
 */
class NonOwningStorage
{
public:

    /**
     * \brief Initializes the storage pointing it to the
     * given object
     *
     * \param object Pointer to the object to reference
     */
    NonOwningStorage(void* object) :
        _reference{object}
    {}

    /**
     * \brief Reassigns the storage to reference another
     * object
     *
     * \param object Pointer to the new object to reference
     */
    void rebind(void* object)
    {
        _reference = object;
    }

    /**
     * \brief Returns a pointer to the storage memory space
     *
     * \param alignment Required storage alignment. Byte boundary by default
     *
     * \returns A pointer to the reference object. Alignment is ignored, this
     * function assumes the pointer to the referenced object is valid
     */
    void* storage(cpp::typeerasure::TypeInfo typeInfo) const
    {
        return _reference;
    }

private:
    void* _reference;
};
}

#endif // SIPLASPLAS_TYPEERASURE_ANYSTORAGE_NONOWNING_HPP
