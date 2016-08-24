#ifndef SIPLASPLAS_UTILITY_ASSERT_HPP
#define SIPLASPLAS_UTILITY_ASSERT_HPP

#include <stdexcept>
#include <functional>
#include "preprocessor.hpp"
#include "exception.hpp"
#include <siplasplas/utility/export.hpp>

namespace cpp
{

/**
 * \ingroup utility
 * \defgroup assert
 * \brief Assertion utilities.
 */

/**
 * \ingroup assert
 * \brief Represents an assertion error
 *
 * An AssertException is thrown whenever a siplasplas assertion fails (See AssertExpression).
 * AssertException::what() returns a detailed message about the assertion failure.
 */
class SIPLASPLAS_UTILITY_EXPORT AssertException : public std::logic_error
{
public:
    using std::logic_error::logic_error;
};

/**
 * \ingroup assert
 * \brief Implements a siplasplas assertion.
 *
 * Siplasplas implements assertions as instances of AssertExpression class.
 * An assert expression object takes the result (true or false) of the assertion,
 * its source location (file, line) and a detailed message.
 * The assertion is evaluated during the destruction of the assertion object. If
 * the assertion failed (i.e. **the value of the result was false**) the AssertExpression
 * destructor throws an AssertException exception.
 */
class SIPLASPLAS_UTILITY_EXPORT AssertExpression
{
public:
    /**
     * \brief Initializes an assertion expression
     *
     * \param expressionResult Result of the assertion expression evaluation. True if the assertion
     * passed, false if the assertion failed.
     * \param message Asseertion error message.
     * \param file Line where the assertion is defined.
     * \param line File where the assertion is defined.
     */
    AssertExpression(bool expressionResult,
                     const std::string& message,
                     const std::string& file,
                     std::size_t line);

    /**
     * \brief Assertion destructor.
     *
     * The assertion result value is checked in the destructor body. If the result was true the
     * assertion is considered passed and the program continues. If the result is false, the
     * onFailure() callback is called (if any) and then an AssertExpression is thrown with detailed
     * information about the assertion failure (Assertion message, location, etc). The assertion stack
     * trace is logged to the error log (See cpp::exception()).
     */
    ~AssertExpression() noexcept(false);

    /**
     * \brief Specifies an action to execute when the assertion fails.
     *
     * This function allows to install a callback on the assertion so an special action
     * can be taken when the assertion fails. The callback is unique, i.e. two consecutive
     * calls to onFailure() overwrite the assigned callback.
     *
     * \param callback Function to execute when the assertion fails.
     *
     * @return Reference to the assertion expresion (*this). Implements a fluent
     * interface on the assertion expression.
     *
     * ``` cpp
     * static std::size_t failures = 0;
     *
     * int sqrt(int n)
     * {
     *      SIPLASPLAS_ASSERT_BT(n, 0).onFailure([&]
     *      {
     *          // Increment the global failures count
     *          // if the assertion fails.
     *          failures++;
     *      });
     *
     *      return 1 / finvsqrt(n);
     * }
     * ```
     *
     * \returns A reference to *this to implement the assertion expression fluent
     * interface.
     */
    AssertExpression& onFailure(const std::function<void()>& callback);

    /**
     * \brief Adds detailed information to the assertion report
     *
     * \param messageBody Detail message string
     * \param messageArgs Detail message arguments. The body may contain placeholders to fill
     * with this values (See fmt::format()).
     *
     * \returns A reference to *this to implement the assertion expression fluent
     * interface.
     */
    template<typename String, typename... Args>
    AssertExpression& detail(String&& messageBody, Args&&... messageArgs)
    {
        _detail = ::fmt::format(std::forward<String>(messageBody), std::forward<Args>(messageArgs)...);
        return *this;
    }

    /**
     * \brief Adds detailed information to the assertion report. Equivalent to detail().
     */
    template<typename String, typename... Args>
    AssertExpression& operator()(String&& messageBody, Args&&... args)
    {
        return detail(std::forward<String>(messageBody), std::forward<Args>(args)...);
    }

private:
    std::string _message;
    std::string _detail;
    std::string _file;
    std::size_t _line;
    std::function<void()> _onFailureCallback;
    bool _assertionFailed;
};

/**
 * \ingroup assert
 * \brief Represents an assert expression when SIPLASPLAS_ASSERT() macros are disabled.
 *
 * When siplasplas assertions are disabled DummyAssertExpression objects are instanced instead,
 * and the expression does nothing.
 */
class SIPLASPLAS_UTILITY_EXPORT DummyAssertExpression
{
public:
    /**
     * \brief Does nothing.
     *
     * This function template is defined only to make siplasplas assertions
     * compile when are disabled. It does nothing.
     *
     * \returns A reference to *this to implement the assertion expression fluent
     * interface.
     */
    template<typename Function>
    DummyAssertExpression& onFailure(Function)
    {
        return *this;
    }

    /**
     * \brief Does nothing
     *
     * This function template is defined only to make siplasplas assertions
     * compile when are disabled. It does nothing.
     *
     * \returns A reference to *this to implement the assertion expression fluent
     * interface.
     */
    template<typename String, typename... Args>
    DummyAssertExpression& detail(String&&, Args&&...)
    {
        return *this;
    }

    /**
     * \brief Does nothing. Equivalent to detail().
     */
    template<typename String, typename... Args>
    DummyAssertExpression& operator()(String&&, Args&&...)
    {
        return *this;
    }
};

}

#if !defined(NDEBUG) || defined(SIPLASPLAS_ENABLE_ASSERTS)
#define SIPLASPLAS_ASSERTS_ENABLED
#define SIPLASPLAS_ASSERT_IMPL(MESSAGE, ...) ::cpp::AssertExpression((__VA_ARGS__), MESSAGE, __FILE__, __LINE__)
#else
#define SIPLASPLAS_ASSERT_IMPL(MESSAGE, ...) ::cpp::DummyAssertExpression()
#endif

/**
 * \ingroup assert
 * \brief Defines an assertion expression.
 *
 * When `NDEBUG` is not defined, this macro is defined to instance a cpp::AssertExpression object.
 * Else it instances a cpp::DummyAssertExpression object which does nothing.
 *
 * \param \__VA_ARGS\__ Parameters for the underlying assertion expression constructor. See cpp::AssertExpression
 * and cpp::DummyAssertExpression.
 *
 * ``` cpp
 * SIPLASPLAS_ASSERT("The answer to the universe and everything must be...", answer == 42);
 * ```
 */
#define SIPLASPLAS_ASSERT(...) SIPLASPLAS_ASSERT_IMPL(SIPLASPLAS_PP_STR((__VA_ARGS__)), __VA_ARGS__)

/**
 * \ingroup assert
 * \brief Implements a comparison assertion.
 *
 * This macro builds a detailed assertion expression for expressions
 * that compare two values using an specific comparison operator.
 * The resulting diagnostic message is of the form:
 *
 * ```
 * Expected '<left expression>' (<left expression value>) <operator wording> '<right expression>' (<right expression value>)
 * ```
 *
 * So for an example assertion like this:
 *
 * ``` cpp
 * void bad_memcpy(void* to, void* from, int bytes)
 * {
 *     constexpr int MIN_BYTES = 0;
 *     SIPLASPLAS_ASSERT_COMP_IMPL(bytes, MIN_BYTES, >=, "bigger or equal to");
 * }
 *
 * char source[1024];
 * char dest[1024];
 * bad_memcpy(buffer, dest, -8);
 * ```
 *
 * returns a diagnostic like the following:
 *
 * ```
 * Expected 'bytes' (-8) bigger or equal to 'MIN_BYTES' (0)
 * ```
 *
 * This macro is used by siplasplas to implement comparison assertion macros for different
 * comparison operators.
 *
 * Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing
 * `operator<<(std::ostream&, const T&)`
 */
#define SIPLASPLAS_ASSERT_COMP_IMPL(a, b, op, wording) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                    "Expected '{}' ({}) {} '{}' ({})",                                       \
                    SIPLASPLAS_PP_STR(a),                                                    \
                    a,                                                                       \
                    wording,                                                                 \
                    SIPLASPLAS_PP_STR(b),                                                    \
                    b                                                                        \
                ), ((a) op (b)))

/**
 * \ingroup assert
 * \brief Defines an equal to assertion.
 *
 * The assertion success if \p a and \p b are equal. Fails otherwise.
 * Equality is tested using `operator==(a, b)`.
 *
 * \param a First value to compare.
 * \param b Second value to compare.
 *
 * ``` cpp
 * int a = 3141592;
 * SIPLASPLAS_ASSERT_EQ(a, 42); // "Expected 'a' (3141592) equal to '42' (42)"
 * ```
 *
 * Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing
 * `operator<<(std::ostream&, const T&)`
 */
#define SIPLASPLAS_ASSERT_EQ(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, ==, "equal to")


/**
 * \ingroup assert
 * \brief Defines a not equal to assertion.
 *
 * The assertion success if \p a and \p b are not equal. Fails otherwise.
 * Inequality is tested using `operator!=(a, b)`.
 *
 * \param a First value to compare.
 * \param b Second value to compare.
 *
 * ``` cpp
 * int a = 42;
 * SIPLASPLAS_ASSERT_NE(a, 42); // "Expected 'a' (42) not equal to '42' (42)"
 * ```
 *
 * Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing
 * `operator<<(std::ostream&, const T&)`
 */
#define SIPLASPLAS_ASSERT_NE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, !=, "not equal to")


/**
 * \ingroup assert
 * \brief Defines a bigger than assertion.
 *
 * The assertion success if \p a is bigger than \p b. Fails otherwise.
 * The comparison is done by means of `operator>(a, b)`.
 *
 * \param a First value to compare.
 * \param b Second value to compare.
 *
 * ``` cpp
 * int a = -3141592;
 * SIPLASPLAS_ASSERT_BT(a, 42); // "Expected 'a' (-3141592) bigger than '42' (42)"
 * ```
 *
 * Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing
 * `operator<<(std::ostream&, const T&)`
 */
#define SIPLASPLAS_ASSERT_BT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, > , "bigger than")


/**
 * \ingroup assert
 * \brief Defines a less than assertion.
 *
 * The assertion success if \p a is less than \p b. Fails otherwise.
 * Comparison is done by means of `operator<(a, b)`.
 *
 * \param a First value to compare.
 * \param b Second value to compare.
 *
 * ``` cpp
 * int a = 3141592;
 * SIPLASPLAS_ASSERT_EQ(a, 42); // "Expected 'a' (3141592) less than '42' (42)"
 * ```
 *
 * Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing
 * `operator<<(std::ostream&, const T&)`
 */
#define SIPLASPLAS_ASSERT_LT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, < , "less than")


/**
 * \ingroup assert
 * \brief Defines a bigger or equal assertion.
 *
 * The assertion success if \p a is bigger or equal to \p b. Fails otherwise.
 * The comparison is done by means of `operator>=(a, b)`.
 *
 * \param a First value to compare.
 * \param b Second value to compare.
 *
 * ``` cpp
 * int a = -3141592;
 * SIPLASPLAS_ASSERT_BT(a, 42); // "Expected 'a' (-3141592) bigger or equal to '42' (42)"
 * ```
 *
 * Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing
 * `operator<<(std::ostream&, const T&)`
 */
#define SIPLASPLAS_ASSERT_BE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, >=, "bigger or equal to")


/**
 * \ingroup assert
 * \brief Defines a less or equal assertion.
 *
 * The assertion success if \p a is less or equal to \p b. Fails otherwise.
 * Comparison is done by means of `operator<=(a, b)`.
 *
 * \param a First value to compare.
 * \param b Second value to compare.
 *
 * ``` cpp
 * int a = 3141592;
 * SIPLASPLAS_ASSERT_EQ(a, 42); // "Expected 'a' (3141592) less or equal to '42' (42)"
 * ```
 *
 * Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing
 * `operator<<(std::ostream&, const T&)`
 */
#define SIPLASPLAS_ASSERT_LE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, <=, "less or equal to")

/**
 * \ingroup assert
 * \brief Defines a true assertion
 *
 * The assertion success if the expression evaluates to true. Fails otherwise.
 *
 * \param ... Assertion boolean expression
 *
 * ``` cpp
 * SIPLASPLAS_ASSERT_TRUE(std::is_integral<std::string>()); // "Expected 'std::is_integral<std::string>()' to be true. Got false instead"
 * ```
 */
#define SIPLASPLAS_ASSERT_TRUE(...) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                "Expected '{}' to be true. Got false instead",            \
                SIPLASPLAS_PP_STR((__VA_ARGS__))                          \
            ), __VA_ARGS__)

/**
 * \ingroup assert
 * \brief Defines a false assertion
 *
 * The assertion success if the expression evaluates to false. Fails otherwise.
 *
 * \param ... Assertion boolean expression
 *
 * ``` cpp
 * SIPLASPLAS_ASSERT_FALSE(std::is_integral<float>()); // "Expected 'std::is_integral<float>()' to be false. Got true instead"
 * ```
 */
#define SIPLASPLAS_ASSERT_FALSE(...) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                "Expected '{}' to be false. Got true instead",            \
                SIPLASPLAS_PP_STR((__VA_ARGS__))                          \
            ), __VA_ARGS__)


#endif // SIPLASPLAS_UTILITY_ASSERT_HPP
