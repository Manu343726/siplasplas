/**
 * \defgroup any storage
 * \ingroup type-erasure
 * \brief Provides different storage backends for
 * cpp::SimpleAny class.
 *
 * cpp::SimpleAny class expects an `Storage` policy as parameter. Storage
 * policies specify where an object is allocated (hosted), by returning
 * the raw address of the object.
 *
 * The minimal interface of an `Storage` class is:
 *
 * ``` cpp
 * void* Storage::storage(std::size_t alignment) const
 * bool Storage::objectFitsInStorage<T>() const
 * ```
 *
 * Classes implementing user-defined storage policies may extend
 * this interface, but cpp::SimpleAny should be able to work using `storage()`
 * function only. `objectFitsInStorage()` function is used for checks and may
 * not be required in release builds.
 */
