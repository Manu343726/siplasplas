#ifndef SIPLASPLAS_UTILITY_FUSION_HPP
#define SIPLASPLAS_UTILITY_FUSION_HPP

#include "tuple.hpp"
#include "meta.hpp"
#include "identity.hpp"

#include <bitset>
#include <sstream>
#include <vector>

namespace cpp
{

namespace detail
{

template<typename Function, typename Arg, typename... Args>
void foreach(Function function, Arg&& arg, Args&&... args)
{
    [](...){}(
        (function(std::forward<Arg>(arg)), 0),
        (function(std::forward<Args>(args)), 0)...
    );
}

template<typename Function>
void foreach(Function)
{}

}

template<typename Arg>
auto foreach(Arg&& arg)
{
    return [arg](auto function)
    {
        return function(arg);
    };
}

template<typename Arg, typename Arg2, typename... Args>
auto foreach(Arg&& arg, Arg2&& arg2, Args&&... args)
{
    return [arg, arg2, args...](auto function)
    {
        ::cpp::detail::foreach(function, arg, arg2, args...);
    };
}

inline auto foreach()
{
    return [](auto){};
}

namespace
{
    template<typename Ts>
    class TypesCall;

    template<typename T, typename... Ts>
    class TypesCall<cpp::meta::list<T, Ts...>>
    {
    public:
        template<typename Function>
        static void apply_void(Function function)
        {
            ::cpp::foreach(
                ::cpp::meta::identity<T>(),
                ::cpp::meta::identity<Ts>()...
            )(function);
        }

        template<typename U, typename Function>
        static std::vector<U> apply(Function function)
        {
            return {
                ::cpp::meta::identity<T>(),
                ::cpp::meta::identity<Ts>()...
            };
        }

        template<typename Function>
        static auto apply(Function function)
        {
            return std::make_tuple(
                ::cpp::meta::identity<T>(),
                ::cpp::meta::identity<Ts>()...
            );
        }
    };

    template<>
    class TypesCall<cpp::meta::list<>>
    {
    public:
        template<typename Function>
        static void apply_void(Function function)
        {}

        template<typename T, typename Function>
        static std::vector<T> apply(Function function)
        {
            return {};
        }

        template<typename Function>
        static auto apply(Function function)
        {
            return std::make_tuple();
        }
    };

    template<typename... Ts>
    class TypesCall<cpp::meta::list<cpp::meta::list<Ts...>>> :
        public TypesCall<meta::list<Ts...>>
    {};
}

template<typename T, typename... Ts, typename Function>
std::vector<T> types_call(Function function)
{
    return TypesCall<cpp::meta::list<Ts...>>::template apply<T>(function);
}

template<typename... Ts, typename Function>
void foreach_type(Function function)
{
    TypesCall<cpp::meta::list<Ts...>>::apply_void(function);
}

template<typename Function, typename Previous>
auto fold(Function function, Previous&& previous)
{
    return std::forward<Previous>(previous);
}

template<typename Function, typename Previous, typename Head, typename... Tail>
auto fold(Function function, Previous&& previous, Head&& head, Tail&&... tail)
{
    return fold(
        function,
        function(
            std::forward<Previous>(previous),
            std::forward<Head>(head)
        ),
        std::forward<Tail>(tail)...
    );
}

namespace
{

template<typename Function, typename Previous, typename... Args, std::size_t... Indices>
auto fold_tuple(Function function, Previous&& previous, const std::tuple<Args...>& tuple, cpp::meta::index_sequence<Indices...>)
{
    return fold(
        function,
        std::forward<Previous>(previous),
        std::get<Indices>(tuple)...
    );
}

template<typename Function, typename Previous, typename... Args, std::size_t... Indices>
auto fold_tuple(Function function, Previous&& previous, std::tuple<Args...>&& tuple, cpp::meta::index_sequence<Indices...>)
{
    return fold(
        function,
        std::forward<Previous>(previous),
        std::move(std::get<Indices>(tuple))...
    );
}

template<typename Function, typename Previous>
auto fold_tuple(Function function, Previous&& previous, const std::tuple<>& tuple, cpp::meta::index_sequence<>)
{
    return std::forward<Previous>(previous);
}

template<typename Function, typename Previous>
auto fold_tuple(Function function, Previous&& previous, std::tuple<>&& tuple, cpp::meta::index_sequence<>)
{
    return std::forward<Previous>(previous);
}

}

template<typename Function, typename Previous, typename... Args>
auto foldTuple(Function function, Previous&& previous, const std::tuple<Args...>& tuple)
{
    return fold_tuple(function, std::forward<Previous>(previous), tuple, cpp::meta::make_index_sequence_for<Args...>());
}

template<typename Function, typename Previous, typename... Args>
auto foldTuple(Function function, Previous&& previous, std::tuple<Args...>&& tuple)
{
    return fold_tuple(function, std::forward<Previous>(previous), std::move(tuple), cpp::meta::make_index_sequence_for<Args...>());
}

template<typename Predicate, typename... Args>
bool anyOf(Predicate predicate, const Args&... args)
{
    return fold([predicate](bool previous, const auto& current)
    {
        return previous || predicate(current);
    }, false, args...);
}

template<typename Predicate, typename... Args>
bool allOf(Predicate predicate, const Args&... args)
{
    return fold([predicate](bool previous, const auto& current)
    {
        return previous && predicate(current);
    }, true, args...);
}

template<typename Predicate, typename... Args>
bool anyOfTuple(Predicate predicate, const std::tuple<Args...>& args)
{
    return fold_tuple([predicate](bool previous, const auto& current)
    {
        return previous || predicate(current);
    }, false, args, cpp::meta::make_index_sequence_for<Args...>());
}

template<typename Predicate, typename... Args>
bool allOfTuple(Predicate predicate, const std::tuple<Args...>& args)
{
    return fold_tuple([predicate](bool previous, const auto& current)
    {
        return previous && predicate(current);
    }, true, args, cpp::meta::make_index_sequence_for<Args...>());
}

template<typename T, typename Function, typename... Args>
std::vector<T> fmap(Function function, Args&&... args)
{
    return {function(std::forward<Args>(args))...};
}

template<typename T, typename... Ts, typename Function>
std::vector<T> fmap(Function function)
{
    return types_call<T, Ts...>(function);
}

}

#endif // SIPLASPLAS_UTILITY_FUSION_HPP
