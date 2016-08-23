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

/**
 * \ingroup utility
 * \brief Checks if an expression is well formed
 *
 * This function takes an expression in the form of an SFINAE-friendly context
 * and checks if the expression can be compiled. For example:
 *
 * ``` cpp
 * auto result = cpp::compiles([](auto identity) -> decltype(
 *     identity(std::declval<std::string>()).append("hello")
 * ) {});
 *
 * static_assert(!result, "std::string has no append() method");
 * ```
 *
 * The cpp::compiles(<test>) expression itself is not constexpr, so two steps
 * are needed to perform the constexpr test. cpp::compiles() returns an implementation
 * defined type that is constexpr-convertible to bool, giving the result of the test.
 *
 * \param expr A callable template of signature `decltype(<expression to check>)(Identity)`.
 * The identity parameter is given to delay the evaluation of the expression, making it dependent
 * of a template of the test. If the expression is directly evaluable, an ill-formed expression would make
 * the program compilation to fail.
 *
 * \returns An implementation defined type constexpr-convertible to bool. The returned value converts
 * to true if the expression is well formed, converts to false otherwise.
 */
template<typename Expr>
constexpr auto compiles(Expr&& expr)
{
    return ::cpp::detail::Compiles<Expr&&>();
}

/**
 * \ingroup utility
 * \brief Checks if an expression is well formed
 *
 * This macro hides the syntax complexity of cpp::compiles() and checks directly
 * whatever expression is passed as argument to the macro:
 *
 * ``` cpp
 * auto result = SIPLASPLAS_UTILITY_COMPILES(
 *     identity(std::declval<std::string>()).append("hello")
 * );
 *
 * static_assert(!result, "std::string has no append() method");
 * ```
 *
 * Note there's an implicit `identity` variable available to delay the evaluation
 * of the expression.
 *
 * \param Expression to check
 *
 * \returns An implementation defined type constexpr-convertible to bool. The returned value converts
 * to true if the expression is well formed, converts to false otherwise.
 */
#define SIPLASPLAS_UTILITY_COMPILES(...)                  \
        ::cpp::compiles(                                  \
            [](auto identity) -> decltype(__VA_ARGS__) {} \
        )

}

#endif // SIPLASPLAS_UTILITY_COMPILES_HPP
