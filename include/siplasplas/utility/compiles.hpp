#ifndef SIPLASPLAS_UTILITY_COMPILES_HPP
#define SIPLASPLAS_UTILITY_COMPILES_HPP

#include "meta.hpp"
#include "identity.hpp"

namespace cpp
{

namespace detail
{

template<typename Expr>
class Compiles
{
private:
    template<typename F, typename = decltype(
        std::declval<F>()(::cpp::Identity())
    )>
    static std::true_type test(void*);

    template<typename F>
    static std::false_type test(...);

public:
    constexpr operator bool() const
    {
        return value;
    }

    static constexpr bool value = decltype(test<Expr>(nullptr))::value;
};

}

template<typename Expr>
constexpr auto compiles(Expr&&)
{
    return ::cpp::detail::Compiles<Expr&&>();
}

#define SIPLASPLAS_UTILITY_COMPILES(...)                  \
        ::cpp::compiles(                                  \
            [](auto identity) -> decltype(__VA_ARGS__) {} \
        )

}

#endif // SIPLASPLAS_UTILITY_COMPILES_HPP
