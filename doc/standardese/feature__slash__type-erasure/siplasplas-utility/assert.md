---
layout: standardese-doc
---

# Header file `assert.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_ASSERT_HPP 

#include <stdexcept>

#include <functional>

#include "preprocessor.hpp"

#include "exception.hpp"

#include <siplasplas/utility/export.hpp>

#define SIPLASPLAS_ASSERT_IMPL(MESSAGE, ...) ::cpp::AssertExpression((__VA_ARGS__), MESSAGE, __FILE__, __LINE__)

#define SIPLASPLAS_ASSERT(...) SIPLASPLAS_ASSERT_IMPL(SIPLASPLAS_PP_STR((__VA_ARGS__)), __VA_ARGS__)

#define SIPLASPLAS_ASSERT_COMP_IMPL(a, b, op, wording) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                    "Expected '{}' ({}) {} '{}' ({})",                                       \
                    SIPLASPLAS_PP_STR(a),                                                    \
                    a,                                                                       \
                    wording,                                                                 \
                    SIPLASPLAS_PP_STR(b),                                                    \
                    b                                                                        \
                ), ((a) op (b)))

#define SIPLASPLAS_ASSERT_EQ(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, ==, "equal to")

#define SIPLASPLAS_ASSERT_NE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, !=, "not equal to")

#define SIPLASPLAS_ASSERT_BT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, > , "bigger than")

#define SIPLASPLAS_ASSERT_LT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, < , "less than")

#define SIPLASPLAS_ASSERT_BE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, >=, "bigger or equal to")

#define SIPLASPLAS_ASSERT_LE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, <=, "less or equal to")

#define SIPLASPLAS_ASSERT_TRUE(...) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                "Expected '{}' to be true. Got false instead",            \
                SIPLASPLAS_PP_STR((__VA_ARGS__))                          \
            ), __VA_ARGS__)

#define SIPLASPLAS_ASSERT_FALSE(...) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                "Expected '{}' to be false. Got true instead",            \
                SIPLASPLAS_PP_STR((__VA_ARGS__))                          \
            ), __VA_ARGS__)

namespace cpp
{
    class AssertException;
    
    class AssertExpression;
    
    class DummyAssertExpression;
}
```

## Macro `SIPLASPLAS_ASSERT`<a id="SIPLASPLAS_ASSERT"></a>

``` cpp
#define SIPLASPLAS_ASSERT(...) SIPLASPLAS_ASSERT_IMPL(SIPLASPLAS_PP_STR((__VA_ARGS__)), __VA_ARGS__)
```

When `NDEBUG` is not defined, this macro is defined to instance a cpp::AssertExpression object. Else it instances a cpp::DummyAssertExpression object which does nothing.

``` cpp
SIPLASPLAS_ASSERT("The answer to the universe and everything must be...", answer == 42);
```

-----

## Macro `SIPLASPLAS_ASSERT_COMP_IMPL`<a id="SIPLASPLAS_ASSERT_COMP_IMPL"></a>

``` cpp
#define SIPLASPLAS_ASSERT_COMP_IMPL(a, b, op, wording) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                    "Expected '{}' ({}) {} '{}' ({})",                                       \
                    SIPLASPLAS_PP_STR(a),                                                    \
                    a,                                                                       \
                    wording,                                                                 \
                    SIPLASPLAS_PP_STR(b),                                                    \
                    b                                                                        \
                ), ((a) op (b)))
```

This macro builds a detailed assertion expression for expressions that compare two values using an specific comparison operator. The resulting diagnostic message is of the form:

    Expected '<left expression>' (<left expression value>) <operator wording> '<right expression>' (<right expression value>)

So for an example assertion like this:

``` cpp
void bad_memcpy(void* to, void* from, int bytes)
{
    constexpr int MIN_BYTES = 0;
    SIPLASPLAS_ASSERT_COMP_IMPL(bytes, MIN_BYTES, >=, "bigger or equal to");
}

char source[1024];
char dest[1024];
bad_memcpy(buffer, dest, -8);
```

returns a diagnostic like the following:

    Expected 'bytes' (-8) bigger or equal to 'MIN_BYTES' (0)

This macro is used by siplasplas to implement comparison assertion macros for different comparison operators.

Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing `operator<<(std::ostream&, const T&)`

-----

## Macro `SIPLASPLAS_ASSERT_EQ`<a id="SIPLASPLAS_ASSERT_EQ"></a>

``` cpp
#define SIPLASPLAS_ASSERT_EQ(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, ==, "equal to")
```

The assertion success if \\p a and \\p b are equal. Fails otherwise. Equality is tested using `operator==(a, b)`.

``` cpp
int a = 3141592;
SIPLASPLAS_ASSERT_EQ(a, 42); // "Expected 'a' (3141592) equal to '42' (42)"
```

Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing `operator<<(std::ostream&, const T&)`

-----

## Macro `SIPLASPLAS_ASSERT_NE`<a id="SIPLASPLAS_ASSERT_NE"></a>

``` cpp
#define SIPLASPLAS_ASSERT_NE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, !=, "not equal to")
```

The assertion success if \\p a and \\p b are not equal. Fails otherwise. Inequality is tested using `operator!=(a, b)`.

``` cpp
int a = 42;
SIPLASPLAS_ASSERT_NE(a, 42); // "Expected 'a' (42) not equal to '42' (42)"
```

Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing `operator<<(std::ostream&, const T&)`

-----

## Macro `SIPLASPLAS_ASSERT_BT`<a id="SIPLASPLAS_ASSERT_BT"></a>

``` cpp
#define SIPLASPLAS_ASSERT_BT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, > , "bigger than")
```

The assertion success if \\p a is bigger than \\p b. Fails otherwise. The comparison is done by means of `operator>(a, b)`.

``` cpp
int a = -3141592;
SIPLASPLAS_ASSERT_BT(a, 42); // "Expected 'a' (-3141592) bigger than '42' (42)"
```

Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing `operator<<(std::ostream&, const T&)`

-----

## Macro `SIPLASPLAS_ASSERT_LT`<a id="SIPLASPLAS_ASSERT_LT"></a>

``` cpp
#define SIPLASPLAS_ASSERT_LT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, < , "less than")
```

The assertion success if \\p a is less than \\p b. Fails otherwise. Comparison is done by means of `operator<(a, b)`.

``` cpp
int a = 3141592;
SIPLASPLAS_ASSERT_EQ(a, 42); // "Expected 'a' (3141592) less than '42' (42)"
```

Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing `operator<<(std::ostream&, const T&)`

-----

## Macro `SIPLASPLAS_ASSERT_BE`<a id="SIPLASPLAS_ASSERT_BE"></a>

``` cpp
#define SIPLASPLAS_ASSERT_BE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, >=, "bigger or equal to")
```

The assertion success if \\p a is bigger or equal to \\p b. Fails otherwise. The comparison is done by means of `operator>=(a, b)`.

``` cpp
int a = -3141592;
SIPLASPLAS_ASSERT_BT(a, 42); // "Expected 'a' (-3141592) bigger or equal to '42' (42)"
```

Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing `operator<<(std::ostream&, const T&)`

-----

## Macro `SIPLASPLAS_ASSERT_LE`<a id="SIPLASPLAS_ASSERT_LE"></a>

``` cpp
#define SIPLASPLAS_ASSERT_LE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, <=, "less or equal to")
```

The assertion success if \\p a is less or equal to \\p b. Fails otherwise. Comparison is done by means of `operator<=(a, b)`.

``` cpp
int a = 3141592;
SIPLASPLAS_ASSERT_EQ(a, 42); // "Expected 'a' (3141592) less or equal to '42' (42)"
```

Compared values must be printable by the [fmt](https://github.com/fmtlib/fmt) library. This can be achieved by implementing `operator<<(std::ostream&, const T&)`

-----

## Macro `SIPLASPLAS_ASSERT_TRUE`<a id="SIPLASPLAS_ASSERT_TRUE"></a>

``` cpp
#define SIPLASPLAS_ASSERT_TRUE(...) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                "Expected '{}' to be true. Got false instead",            \
                SIPLASPLAS_PP_STR((__VA_ARGS__))                          \
            ), __VA_ARGS__)
```

The assertion success if the expression evaluates to true. Fails otherwise.

``` cpp
SIPLASPLAS_ASSERT_TRUE(std::is_integral<std::string>()); // "Expected 'std::is_integral<std::string>()' to be true. Got false instead"
```

-----

## Macro `SIPLASPLAS_ASSERT_FALSE`<a id="SIPLASPLAS_ASSERT_FALSE"></a>

``` cpp
#define SIPLASPLAS_ASSERT_FALSE(...) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                "Expected '{}' to be false. Got true instead",            \
                SIPLASPLAS_PP_STR((__VA_ARGS__))                          \
            ), __VA_ARGS__)
```

The assertion success if the expression evaluates to false. Fails otherwise.

``` cpp
SIPLASPLAS_ASSERT_FALSE(std::is_integral<float>()); // "Expected 'std::is_integral<float>()' to be false. Got true instead"
```

-----

## Class `cpp::AssertException`<a id="cpp::AssertException"></a>

``` cpp
class AssertException
: public std::logic_error
{
public:
    using std::logic_error::logic_error;
};
```

An AssertException is thrown whenever a siplasplas assertion fails (See AssertExpression). AssertException::what() returns a detailed message about the assertion failure.

-----

## Class `cpp::AssertExpression`<a id="cpp::AssertExpression"></a>

``` cpp
class AssertExpression
{
public:
    AssertExpression(bool expressionResult, const std::string& message, const std::string& file, std::size_t line);
    
    ~AssertExpression() noexcept(false);
    
    AssertExpression& onFailure(const std::function<void()>& callback);
};
```

Siplasplas implements assertions as instances of AssertExpression class. An assert expression object takes the result (true or false) of the assertion, its source location (file, line) and a detailed message. The assertion is evaluated during the destruction of the assertion object. If the assertion failed (i.e. **the value of the result was false**) the AssertExpression destructor throws an AssertException exception.

### Constructor `cpp::AssertExpression::AssertExpression`<a id="cpp::AssertExpression::AssertExpression"></a>

``` cpp
AssertExpression(bool expressionResult, const std::string& message, const std::string& file, std::size_t line);
```

Initializes an assertion expression

#### Parameter `cpp::AssertExpression::AssertExpression::expressionResult`<a id="cpp::AssertExpression::AssertExpression::expressionResult"></a>

``` cpp
bool expressionResult
```

Result of the assertion expression evaluation. True if the assertion passed, false if the assertion failed. \\param message Asseertion error message. \\param file Line where the assertion is defined. \\param line File where the assertion is defined.

-----

### Destructor `cpp::AssertExpression::~AssertExpression`<a id="cpp::AssertExpression::~AssertExpression"></a>

``` cpp
~AssertExpression() noexcept(false);
```

Assertion destructor.

The assertion result value is checked in the destructor body. If the result was true the assertion is considered passed and the program continues. If the result is false, the onFailure() callback is called (if any) and then an AssertExpression is thrown with detailed information about the assertion failure (Assertion message, location, etc). The assertion stack trace is logged to the error log (See cpp::exception()).

-----

### Function `cpp::AssertExpression::onFailure`<a id="cpp::AssertExpression::onFailure"></a>

``` cpp
AssertExpression& onFailure(const std::function<void()>& callback);
```

Specifies an action to execute when the assertion fails.

This function allows to install a callback on the assertion so an special action can be taken when the assertion fails. The callback is unique, i.e. two consecutive calls to onFailure() overwrite the assigned callback.

@return Reference to the assertion expresion (\*this). Implements a fluent interface on the assertion expression.

``` cpp
static std::size_t failures = 0;

int sqrt(int n)
{
     SIPLASPLAS_ASSERT_BT(n, 0).onFailure([&]
     {
         // Increment the global failures count
         // if the assertion fails.
         failures++;
     });

     return 1 / finvsqrt(n);
}
```

#### Parameter `cpp::AssertExpression::onFailure::callback`<a id="cpp::AssertExpression::onFailure::callback"></a>

``` cpp
const std::function<void()>& callback
```

Function to execute when the assertion fails.

-----

-----

## Class `cpp::DummyAssertExpression`<a id="cpp::DummyAssertExpression"></a>

``` cpp
class DummyAssertExpression
{
public:
    template <typename Function>
    DummyAssertExpression& onFailure(Function);
};
```

When siplasplas assertions are disabled DummyAssertExpression objects are instanced instead, and the expression does nothing.

### Function template `cpp::DummyAssertExpression::onFailure<Function>`<a id="cpp::DummyAssertExpression::onFailure<Function>"></a>

``` cpp
template <typename Function>
DummyAssertExpression& onFailure(Function);
```

Does nothing.

This function template is defined only to make siplasplas assertions compile when are disabled. It does nothing.

@return A reference to \*this to implement the assertion expression fluent interface.

#### Template parameter `cpp::DummyAssertExpression::onFailure<Function>::Function`<a id="cpp::DummyAssertExpression::onFailure<Function>::Function"></a>

``` cpp
typename Function
```

-----

-----
