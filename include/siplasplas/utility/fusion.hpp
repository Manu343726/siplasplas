#ifndef SIPLASPLAS_UTILITY_FUSION_HPP
#define SIPLASPLAS_UTILITY_FUSION_HPP

#include "tuple.hpp"
#include "meta.hpp"

#include <bitset>
#include <sstream>

#include <ctti/type_id.hpp>

namespace cpp
{

namespace detail
{

template<typename T>
class DefaultConstructible
{
public:
    constexpr DefaultConstructible() = default;

    using type = T;
};

template<typename T>
constexpr DefaultConstructible<T> defaultConstructible = DefaultConstructible<T>();

template<typename Function, typename... Args>
void foreach(Function function, Args&&... args)
{
    [](...){}((function(std::forward<Args>(args)), 0)...);
}

}

template<typename... Args>
auto foreach(Args&&... args)
{
    return [args...](auto function)
    {
        ::cpp::detail::foreach(function, args...);
    };
}

namespace
{
    template<typename Ts>
    class CallForeach;

    template<typename... Ts>
    class CallForeach<meta::list<Ts...>>
    {
    public:
        template<typename Function>
        static void apply(Function function)
        {
            ::cpp::foreach(detail::defaultConstructible<Ts>...)(function);
        }
    };

    template<typename... Ts>
    class CallForeach<meta::list<meta::list<Ts...>>> :
        public CallForeach<meta::list<Ts...>>
    {};
}

template<typename... Ts, typename Function>
void foreach(Function function)
{
    CallForeach<meta::list<Ts...>>::apply(function);
}

template<typename Types>
std::string printTypeList()
{
    std::ostringstream os;

    os << "{";

    foreach<Types>([&os](auto type)
    {
        os << ctti::type_id<typename decltype(type)::type>().name() << ", ";
    });

    os << "}";

    return os.str();
}

}

#endif // SIPLASPLAS_UTILITY_FUSION_HPP
