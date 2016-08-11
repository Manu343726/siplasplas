#ifndef SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP
#define SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP

#include <siplasplas/utility/staticif.hpp>
#include <siplasplas/utility/exception.hpp>
#include <siplasplas/utility/tuple.hpp>
#include <siplasplas/typeerasure/concepts/valuesemantics.hpp>

namespace cpp
{

namespace features
{

class DefaultConstructible
{
public:
    template<typename T>
    static T apply() noexcept(concepts::DefaultConstructible<T>::no_except)
    {
        return cpp::staticIf<concepts::DefaultConstructible<T>::value>([](auto identity)
        {
            return identity.type<T>([](auto type)
            {
                return meta::type_t<decltype(type)>();
            });
        }).Else([](auto) -> T
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not default constructible",
                ctti::type_id<T>().name()
            );
        });
    }
};


class Constructible
{
public:
    template<typename T, typename... Args>
    static T apply(Args&&... args) noexcept(concepts::Constructible<T, std::decay_t<Args>...>::no_except)
    {
        // Cannot believe this is the best way to perfect-capture a variadic pack...

        return cpp::staticIf<concepts::Constructible<T, std::decay_t<Args>...>::value>([args = std::forward_as_tuple(std::forward<Args>(args)...)](auto identity)
        {
            // Delay evaluation of T:
            return identity.type<T>([&args](auto type)
            {
                return cpp::tuple_call(args, [type](auto... args)
                {
                    return meta::type_t<decltype(type)>(std::forward<decltype(args)>(args)...);
                });
            });
        }).Else([](auto) -> T
        {
            throw cpp::exception<std::runtime_error>(
                "Type '{}' is not default constructible",
                ctti::type_id<T>().name()
            );
        });
    }
};

}

}

#endif // SIPLASPLAS_TYPEERASURE_FEATURES_VALUESEMANTICS_HPP
