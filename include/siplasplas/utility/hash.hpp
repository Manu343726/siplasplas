#ifndef SIPLASPLAS_UTILITY_HASH_HPP
#define SIPLASPLAS_UTILITY_HASH_HPP

#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include "tuple.hpp"

namespace cpp
{

template<typename T>
constexpr std::size_t hash_combine(std::size_t seed, const T& value)
{
    std::hash<T> hasher;

    return seed ^ (hasher(value) + 0x9e3779b9 + (seed<<6) + (seed>>2));
}

template<typename T>
constexpr std::size_t hash(const T& value)
{
    return std::hash<T>{}(value);
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

}

#endif // SIPLASPLAS_UTILITY_HASH_HPP
