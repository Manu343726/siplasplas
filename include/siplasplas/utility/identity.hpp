#ifndef SIPLASPLAS_UTILITY_IDENTITY_HPP
#define SIPLASPLAS_UTILITY_IDENTITY_HPP

#include "meta.hpp"
#include <utility>

namespace cpp
{

/**
 * \ingroup utility
 * \brief A functor class implementing the identity function
 *
 * The identity function takes a value of any type and returns it as is.
 * The function performs no mutation of the value. Given an expression `x`
 * `decltype(x) == decltype(Identity()(x))` and the yield value is the same.
 * This function is useful to delay the evaluation of an expression to the
 * second template processing phase (The on instantiation phase).
 * See cpp::staticIf or cpp::compiles.
 */
class Identity
{
public:
    template<typename T>
    constexpr auto operator()(T&& value) -> decltype(std::forward<T>(value)) const
    {
        return std::forward<T>(value);
    }

    template<typename T, typename Function>
    constexpr auto type(Function callback) -> decltype(callback(meta::identity<T>())) const
    {
        return callback(meta::identity<T>());
    }

    template<typename T>
    constexpr auto type() const
    {
        return meta::identity<T>();
    }
};

inline auto applyIdentity(const Identity& identity)
{
    return [identity](auto&& value)
    {
        return identity(std::forward<decltype(value)>(value));
    };
}

template<typename T, typename Identity>
constexpr auto applyIdentity(const Identity& identity) -> meta::type_t<decltype(identity.template type<T>())>;

}

#endif // SIPLASPLAS_UTILITY_IDENTITY_HPP
