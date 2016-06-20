#ifndef SIPLASPLAS_UTILITY_HASH_HPP
#define SIPLASPLAS_UTILITY_HASH_HPP

#include "tuple.hpp"
#include "function_traits.hpp"

#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <memory>
#include <cstring>

namespace cpp
{

template<typename T>
constexpr std::size_t hash(const T& value);
template<typename T, typename U, typename... Args>
constexpr std::size_t hash(const T& first, const U& second, const Args&... tail);
template<typename T>
std::size_t raw_hash(const T& value);

namespace
{

template<typename T, cpp::FunctionKind Kind = cpp::function_kind<T>()>
class HashDispatch
{
public:
    static constexpr std::size_t apply(const T& value)
    {
        return std::hash<T>{}(value);
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
            return ::cpp::hash(&value, &T::operator());
        }
    }
};

}

template<typename T>
constexpr std::size_t hash_combine(std::size_t seed, const T& value)
{
    std::hash<T> hasher;

    return seed ^ (hasher(value) + 0x9e3779b9 + (seed<<6) + (seed>>2));
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

template<typename T>
struct Hash
{
public:
    constexpr std::size_t operator()(const T& value) const
    {
        return ::cpp::hash(value);
    }
};

template<typename Key, typename Value>
using HashTable = std::unordered_map<Key, Value, Hash<Key>>;
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
