#ifndef SIPLASPLAS_UTILITY_ASSERT_HPP
#define SIPLASPLAS_UTILITY_ASSERT_HPP

#include <stdexcept>
#include <functional>
#include "preprocessor.hpp"
#include "exception.hpp"

namespace cpp
{

class AssertException : public std::logic_error
{
public:
    using std::logic_error::logic_error;
};

class AssertExpression
{
public:
    AssertExpression(bool expressionResult,
                     const std::string& message,
                     const std::string& file,
                     std::size_t line);
    ~AssertExpression() noexcept(false);

    AssertExpression& onFailure(const std::function<void()>& callback);

private:
    std::string _message;
    std::string _file;
    std::size_t _line;
    std::function<void()> _onFailureCallback;
    bool _assertionFailed;
};

class DummyAssertExpression
{
public:
    template<typename Function>
    DummyAssertExpression& onFailure(Function)
    {
        return *this;
    }
};

}

#ifndef NDEBUG
#define SIPLASPLAS_ASSERT_IMPL(MESSAGE, ...) ::cpp::AssertExpression((__VA_ARGS__), MESSAGE, __FILE__, __LINE__)
#else
#define SIPLASPLAS_ASSERT_IMPL(MESSAGE, ...) ::cpp::DummyAssertExpression()
#endif

#define SIPLASPLAS_ASSERT(...) SIPLASPLAS_ASSERT_IMPL(SIPLASPLAS_PP_STR((__VA_ARGS__)), __VA_ARGS__)

#define SIPLASPLAS_ASSERT_COMP_IMPL(a, b, op, wording) SIPLASPLAS_ASSERT_IMPL(::fmt::format( \
                    "Expected '{}' ({}) {} '{}' ({})",                                       \
                    SIPLASPLAS_PP_STR(a),                                                    \
                    a,                                                                       \
                    wording,                                                                 \
                    SIPLASPLAS_PP_STR(b),                                                    \
                    b                                                                        \
                ), (a op b))

#define SIPLASPLAS_ASSERT_EQ(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, ==, "equal to")
#define SIPLASPLAS_ASSERT_NE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, !=, "not equal to")
#define SIPLASPLAS_ASSERT_BT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, > , "bigger than")
#define SIPLASPLAS_ASSERT_LT(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, < , "less than")
#define SIPLASPLAS_ASSERT_BE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, >=, "bigger or equal to")
#define SIPLASPLAS_ASSERT_LE(a, b) SIPLASPLAS_ASSERT_COMP_IMPL(a, b, <=, "less or equal to")

#endif // SIPLASPLAS_UTILITY_ASSERT_HPP
