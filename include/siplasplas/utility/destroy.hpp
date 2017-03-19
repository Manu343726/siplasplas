#ifndef SIPLASPLAS_UTILITY_DESTROY_HPP
#define SIPLASPLAS_UTILITY_DESTROY_HPP

#include <type_traits>

namespace cpp
{

/**
 * \ingroup utility
 * \brief Invokes the destructor of an object
 *
 * \tparam T Must be Destructible (See std::is_destructible)
 */
template<typename T>
void destroy(T& object)
{
    object.~T();
}

/**
 * \ingroup utility
 * \brief Invokes the destructor of an object
 *
 * \tparam T Must be Destructible (See std::is_destructible)
 */
template<typename T>
void destroy(T* objectPtr)
{
    objectPtr->~T();
}

/**
 * \ingroup utility
 * \brief Invokes the destructor of an object allocated
 * at the given storage address
 *
 * \tparam T Must be Destructible (See std::is_destructible)
 */
template<typename T>
void destroy(void* objPtr)
{
    reinterpret_cast<T*>(objPtr)->~T();
}

template<typename R, typename... Args>
void destroy(R(*functionPointer)(Args...))
{
 // NOOP
}

/**
 * \ingroup utility
 * \brief constructs an object of type T on the specified address
 *
 * This function performs an in-place construction of an object of type
 * T in the given address. Arguments are passed as-is to the object constructor.
 * The behavior is undefined if `alignment(pointer) != alignof(T)`.
 *
 * \param where Location of the object
 * \param args Constructor arguments
 *
 * ``` cpp
 * std::aligned_storage<std::string> storage;
 * cpp::construct<std::string>(&storage, "hello, world!");
 * ```
 */
template<typename T, typename... Args>
void construct(void* where, Args&&... args)
{
    new(where) T(std::forward<Args>(args)...);
}

/**
 * \ingroup utility
 * \brief constructs an object of type T on the specified address
 *
 * This function performs an in-place construction of an object of type
 * T in the given address. Arguments are passed as-is to the object constructor.
 * The behavior is undefined if `alignment(pointer) != alignof(T)`.
 *
 * \param where Location of the object
 * \param args Constructor arguments
 *
 * ``` cpp
 * std::aligned_storage<std::string> storage;
 * std::string* stringPtr = reinterpret_cast<std::string*>(&storage);
 *
 * cpp::construct(stringPtr, "hello, world!");
 * ```
 */
template<typename T, typename... Args>
void construct(T* where, Args&&... args)
{
    construct<std::decay_t<T>>(static_cast<void*>(where), std::forward<Args>(args)...);
}

}

#endif // SIPLASPLAS_UTILITY_DESTROY_HPP
