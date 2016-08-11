#ifndef SIPLASPLAS_UTILITY_STATICIF_HPP
#define SIPLASPLAS_UTILITY_STATICIF_HPP

#include "type_variables.hpp"

namespace cpp
{

namespace detail
{

/**
 * \brief A functor class implementing the identity function
 *
 * The identity function takes a value of any type and returns it as is.
 * The function performs no mutation of the value. Given an expression `x`
 * `decltype(x) == decltype(Identity()(x))` and the yield value is the same.
 * This function is useful to delay the evaluation of an expression to the
 * second template processing phase (The on instantiation phase).
 * See cpp::staticIf.
 */
class Identity
{
public:
    template<typename T>
    constexpr auto operator()(T&& value) -> decltype(std::forward<T>(value))
    {
        return std::forward<T>(value);
    }

    template<typename T, typename Function>
    constexpr auto type(Function callback) -> decltype(callback(type<T>()))
    {
        return callback(cpp::type<T>());
    }
};

/**
 * \brief Implements the then branch of an static conditional.
 *
 * The cpp::detail::If template implements the internals of an `static_if()`.
 * The main template implements the true condition branch (The branch executing the then
 * body). Returning values in this case may not be as efficient as possible since the returned
 * value should bypass the then body (This involves two move operations, see cpp::If::ElseBypass):
 *
 * ``` cpp
 * std::string str = If<true>().Then([](auto identity)
 * {
 *     return "hello, world!"s; // the string is moved twice
 *                              // to exit the static if.
 * });
 * ```
 *
 * \tparam Condition If condition. True in the main template.
 */
template<bool Condition>
class If
{
public:
    template<typename T>
    class ElseBypass
    {
    public:
        constexpr ElseBypass(T&& value) :
            _value{std::move(value)}
        {}

        template<typename Body>
        constexpr T Else(const Body&)
        {
            return static_cast<T>(*this);
        }

        constexpr operator T()
        {
            return std::move(_value);
        }

    private:
        T _value;
    };

    template<typename T>
    class ElseBypass<T&>
    {
    public:
        constexpr ElseBypass(T& value) :
            _value{&value}
        {}

        template<typename Body>
        constexpr T& Else(const Body&)
        {
            return static_cast<T&>(*this);
        }

        constexpr operator T&()
        {
            return *_value;
        }

    private:
        T* _value;
    };

    template<typename Body>
    constexpr typename std::enable_if<
        !std::is_void<decltype(std::declval<Body>()(Identity()))>::value,
        ElseBypass<decltype(std::declval<Body>()(Identity()))>
    >::type
    Then(const Body& body)
    {
        return body(Identity());
    }

    template<typename Body>
    constexpr typename std::enable_if<
        std::is_void<decltype(std::declval<Body>()(Identity()))>::value
    >::type
    Then(const Body& body)
    {
        body(Identity());
    }


    template<typename Body>
    constexpr void Else(const Body&)
    {
        /* NOTHING */
    }
};

template<>
class If<false>
{
public:
    constexpr If() = default;

    template<typename Body>
    constexpr If& Then(const Body&)
    {
        return *this;
    }

    template<typename Body>
    constexpr decltype(auto) Else(const Body& body)
    {
        return body(Identity());
    }
};

}

/**
 * \ingroup utility
 * \brief Implements an static conditional
 *
 * An static conditional allows to conditionally
 * evaluate some code depending on the value of a compile
 * time property. The body of the conditional is implemented
 * by user provided functions.
 *
 * ``` cpp
 * template<typename T>
 * void f(T value)
 * {
 *     cpp::staticIf<std::is_integral<T>::value>([&](auto identity)
 *     {
 *         std::cout << "the value is integral\n";
 *     });
 *
 *     ...
 * }
 * ```
 *
 * The body of the untaken conditional path shall not be
 * evaluated by the compiler and can contain ill formed code.
 * **NOTE**: This behavior relies on the two phase template processing
 * scheme, so the statement above is only true for entities that
 * will be evaluated in the instantiation phase only:
 *
 * ``` cpp
 * template<typename T>
 * void foo(T value)
 * {
 *    int i = 0;
 *
 *    // The condition is false, so the code
 *    // inside the if "should" not be evaluated:
 *    cpp::staticIf<false>([&](auto identity)
 *    {
 *        // Ok: value depends on T template parameter.
 *        value.TheMostBizarreMethodName();
 *
 *        // ERROR: 'int' type is not class/struct/union.
 *        i.method();
 *
 *        // Ok: identity(i) depends on a template parameter.
 *        identity(i).method();
 *    });
 * }
 * ```
 *
 * As the example shows, the conditional body takes an `identity`
 * parameter that can be used to force the evaluation of an expression
 * at the second phase.
 *
 * The static conditional expression also provides an `else` sentence
 * in the form of an `Else()` method:
 *
 * ``` cpp
 * template<typename T>
 * void foo(T value)
 * {
 *    int i = 0;
 *
 *    cpp::staticIf<false>([&](auto identity)
 *    {
 *        value.TheMostBizarreMethodName();
 *        i.method();
 *        identity(i).method();
 *    }).Else([&](auto identity)
 *    {
 *        std::cout << i << std::endl;
 *    });
 * }
 * ```
 *
 * cpp::staticIf() supports returning values from the conditional
 * body too:
 *
 * ``` cpp
 * template<typename T>
 * T twice(const T& value)
 * {
 *     return cpp::staticIf<std::is_class<T>::value>([&](auto)
 *     {
 *         return value.twice();
 *     }).Else([&](auto)
 *     {
 *         return value * 2;
 *     });
 * }
 * ```
 *
 * note this has some caveats:
 *
 *  - **Return types cannot be inferred**: Due to technical
 *    reasons `auto` or any other kind of type inference cannot
 *    be used with the return value of a static conditional.
 *
 *  - **Return may not be optimal in some code paths**: If the condition
 *    is true, the `then` path is picked. If the `then` body returns a value,
 *    such value is not returned directly (So elligible for RVO) but bypassed
 *    through the `else` internals. This means **returning a value from a
 *    positive conditional may involve two move operations**.
 *
 * \tparam Condition Value of the condition. The value should be evaluable
 * at compile time, else compilation fails.
 * \tparam ThenBody Function type with one template-dependent value parameter.
 * \param thenBody Body of the `then` path of the conditional. Evaluated
 * only if the condition is true.
 * \return An unspecified type implementing the `else` part of the
 * conditional.
 */
template<bool Condition, typename ThenBody>
auto staticIf(const ThenBody& thenBody)
{
    detail::If<Condition> if_;
    return if_.Then(thenBody);
}

}

#endif // SIPLASPLAS_UTILITY_STATICIF_HPP
