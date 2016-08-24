#include "assert.hpp"
#include "exception.hpp"

using namespace cpp;

AssertExpression::AssertExpression(bool assertionResult,
                                   const std::string& message,
                                   const std::string& file,
                                   std::size_t line) :
    _message{message},
    _detail{""},
    _file{file},
    _line{line},
    _assertionFailed{!assertionResult}
{}

AssertExpression& AssertExpression::onFailure(const std::function<void()>& callback)
{
    _onFailureCallback = callback;
    return *this;
}

AssertExpression::~AssertExpression() noexcept(false)
{
    if(_assertionFailed)
    {
        if(_onFailureCallback)
        {
            _onFailureCallback();
        }

        if(_detail.empty())
        {
            throw cpp::exception<AssertException>(
                "File {}, line {} Assertion Failed: {}",
                _file,
                _line,
                _message
            );
        }
        else
        {
            throw cpp::exception<AssertException>(
                "File {}, line {} Assertion Failed: {}\nDetails: {}",
                _file,
                _line,
                _message,
                _detail
            );
        }
    }
}
