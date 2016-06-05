#ifndef SIPLASPLAS_UTILITY_TUPLE_HPP
#define SIPLASPLAS_UTILITY_TUPLE_HPP

#include <tuple>
#include "meta.hpp"

namespace cpp
{

namespace
{

template<typename Function, typename... Args, std::size_t... Is>
constexpr auto tuple_call(Function function, const std::tuple<Args...>& tuple, meta::index_sequence<Is...>)
{
    return function(std::forward<Args>(std::get<Is>(tuple))...);
}

template<typename Head, typename... Tail, std::size_t... Is>
constexpr auto tuple_tail(const std::tuple<Head, Tail...>& tuple, std::index_sequence<Is...>)
{
    return std::make_tuple(std::forward<std::tuple_element_t<Is+1, std::tuple<Head, Tail>>>(std::get<Is+1>(tuple))...);
}

}

template<typename Function, typename... Args>
constexpr auto tuple_call(Function function, const std::tuple<Args...>& tuple)
{
    return tuple_call(function, tuple, meta::make_index_sequence_for<Args...>{});
}

template<typename Function, typename... Args>
constexpr auto tuple_call(const std::tuple<Args...>& tuple, Function function)
{
    return tuple_call(function, tuple);
}


template<typename Head, typename... Tail>
constexpr auto tuple_tail(const std::tuple<Head, Tail...>& tuple)
{
    return tuple_tail(tuple, std::index_sequence_for<Tail...>{});
}

}

#endif // SIPLASPLAS_UTILITY_TUPLE_HPP
