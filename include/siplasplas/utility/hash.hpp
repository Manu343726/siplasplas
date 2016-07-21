#ifndef SIPLASPLAS_UTILITY_HASH_HPP
#define SIPLASPLAS_UTILITY_HASH_HPP

#include "tuple.hpp"
#include "function_traits.hpp"

#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <memory>
#include <cstring>

/**
 * \defgroup hash
 * \ingroup utility
 * \brief Hashing utilities based on `std::hash`.
 *
 * This implements the following hashing tools:
 *
 *  - **Free hash function**: A cpp::hash() free function template for simple
 *  hash code retrieval. Because `std::hash<int>()(42)` is horrible.
 *  - **Hash combination**: cpp::hash() is really a variadic template function that implements
 *  pairwise hash combination.
 *  - **Improved hash functor**: cpp::Hash implements a hash functor for maps that wraps
 *  cpp::hash() functionallity. The extended functionallity of cpp::Hash over `std::hash`
 *  includes hashing of enum types, pairs, and tuples out of the box.
 *  - **Hashing of function pointers**: Because we love undefined behavior!
 */

namespace cpp
{

/**
 * \ingroup hash
 * \brief Implements a hash function for values of type T.
 *
 * \param value Value to compute the hash code.
 *
 * \return For enumeration types returns the hash of the underlying integral value.
 * For function types returns a bytewise hash of the function object (See cpp::raw_hash()).
 * For tuple and pair types returns a hash combination of all tuple elements (See cpp::hash_combine() and
 * n-ary cpp::hash() overload).
 * Otherwise returns the value given by `std::hash<T>`.
 */
template<typename T>
constexpr std::size_t hash(const T& value);

/**
 * \ingroup hash
 * \brief Returns the comination of hashes of a set of values.
 *
 * This function is a generalization of unary cpp::hash() that accepts two or more input
 * values. The resulting hash code is computed as the hash combination (See cpp::hash_combine())
 * of the first value and the hash combination of the rest:
 *
 * ``` haskell
 * hash first:second:tail -> hash_combine (hash first) (hash second:tail)
 * ```
 *
 * \param values A variadic pack of at least two elements. These are the values
 * to get the combined hash code.
 *
 * \return Pairwise hash combination of the values. The hash of each value is computed by cpp::hash().
 */
template<typename T, typename U, typename... Args>
constexpr std::size_t hash(const T& first, const U& second, const Args&... tail);

/**
 * \ingroup hash
 * \brief Returns a bytewise hash of a given value.
 *
 * This function ignores the `std::hash` specialization of the value type and implements
 * a bytewise hash value instead. Bytewise hash is computed as a hash combination of each byte
 * of the value storage, in the range `[addressof(value), addressof(value) + sizeof(T))`. The
 * value is copyed to an intermediary aligned storage to perform the byte traversal.
 *
 * \return A value equivalent to `cpp::hash(<bytes of value>...)`.
 */
template<typename T>
std::size_t raw_hash(const T& value);

namespace
{

template<typename T, cpp::FunctionKind Kind = cpp::function_kind<T>()>
class HashDispatch
{
public:
    template<typename U, typename = void>
    class HashDispatchForValueTypes
    {
    public:
        static constexpr std::size_t apply(const U& value)
        {
            return std::hash<T>{}(value);
        }
    };

    template<typename U>
    class HashDispatchForValueTypes<U, typename std::enable_if<std::is_enum<U>::value>::type>
    {
    public:
        static constexpr std::size_t apply(const U& value)
        {
            return std::hash<T>{}(static_cast<typename std::underlying_type<U>::type>(value));
        }
    };

    static constexpr std::size_t apply(const T& value)
    {
        return HashDispatchForValueTypes<T>::apply(value);
    }
};

template<typename T>
class HashDispatch<T, cpp::FunctionKind::FREE_FUNCTION>
{
public:
    static constexpr std::size_t apply(const T& value)
    {
        return ::cpp::raw_hash(value);
    }
};

template<typename T>
class HashDispatch<T, cpp::FunctionKind::MEMBER_FUNCTION>
{
public:
    static constexpr std::size_t apply(const T& value)
    {
        return ::cpp::raw_hash(value);
    }
};

template<typename T>
class HashDispatch<T, cpp::FunctionKind::CONST_MEMBER_FUNCTION>
{
public:
    static constexpr std::size_t apply(const T& value)
    {
        return ::cpp::raw_hash(value);
    }
};

template<typename T>
class HashDispatch<T, cpp::FunctionKind::FUNCTOR>
{
public:
    static constexpr std::size_t apply(const T& value)
    {
        if(std::is_empty<T>::value)
        {
            return ::cpp::hash(&T::operator());
        }
        else
        {
            return ::cpp::hash(::cpp::raw_hash(value), &T::operator());
        }
    }
};

}

/**
 * \ingroup hash
 * \brief Implements a hash combination function of a given hash value and a value of type T.
 *
 * Literally copied from [this stack overflow thread](http://stackoverflow.com/questions/7110301/generic-hash-for-tuples-in-unordered-map-unordered-set)
 * which in turn got it from [`boost::hash_combine()`](http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html).
 *
 * \param seed Source hash value to combine.
 * \param value Value of type T wich has will be combined.
 * \return the combination between the seed value and the hash of the input value. See `boost::hash_combine()` link
 * above for the specific combination function.
 */
template<typename T>
constexpr std::size_t hash_combine(std::size_t seed, const T& value)
{
    return seed ^ (hash(value) + 0x9e3779b9 + (seed<<6) + (seed>>2));
}

template<typename T>
constexpr std::size_t hash(const T& value)
{
    return HashDispatch<T>::apply(value);
}

template<typename T, typename U, typename... Args>
constexpr std::size_t hash(const T& first, const U& second, const Args&... tail)
{
    return hash_combine(
        hash(first),
        hash(second, tail...)
    );
}

template<typename T, typename U>
constexpr std::size_t hash(const std::pair<T, U>& pair)
{
    return hash(pair.first, pair.second);
}

template<typename... Ts>
constexpr std::size_t hash(const std::tuple<Ts...>& tuple)
{
    return cpp::tuple_call(
        tuple,
        [](const Ts&... args)
        {
            return hash(args...);
        }
    );
}

/**
 * \ingroup hash
 * \brief A functor that implements a hash function for values of type T.
 *
 * This template provides the features of cpp::hash() as a functor template,
 * suitable for unordered containers.
 *
 * \tparam T Type of the values to get the hash code. See cpp::hash() for behavior and type
 * requirements.
 */
template<typename T>
struct Hash
{
public:
    constexpr std::size_t operator()(const T& value) const
    {
        return ::cpp::hash(value);
    }
};

/**
 * \ingroup hash
 * \brief `std::unordered_map` alias using cpp::Hash as hash.
 *
 * \tparam Key Key type.
 * \tparam Value Value type.
 */
template<typename Key, typename Value>
using HashTable = std::unordered_map<Key, Value, Hash<Key>>;

/**
 * \ingroup hash
 * \brief `std::set` alias using cpp::Hash as hash.
 *
 * \tparam Key Key type.
 */
template<typename Key>
using HashSet = std::unordered_set<Key, Hash<Key>>;

template<typename T>
std::size_t raw_hash(const T& value)
{
    std::aligned_storage_t<sizeof(T), alignof(T)> rawStorage;
    std::memcpy(&rawStorage, &value, sizeof(T));
    std::size_t hash = 0;

    for(std::size_t i = 0; i < sizeof(T); ++i)
    {
        hash = hash_combine(hash, reinterpret_cast<const char*>(&rawStorage)[i]);
    }

    return hash;
}

/**
 * \ingroup hash
 * \brief A functor that implements a bytewise hash function for values of type T.
 *
 * This template provides the features of cpp::raw_hash() as a functor template,
 * suitable for unordered containers.
 *
 * \tparam T Type of the values to get the hash code. See cpp::raw_hash() for behavior and type
 * requirements.
 */
template<typename T>
class RawHash
{
public:
    constexpr std::size_t operator()(const T& value) const
    {
        return raw_hash(value);
    }
};

}

#endif // SIPLASPLAS_UTILITY_HASH_HPP
