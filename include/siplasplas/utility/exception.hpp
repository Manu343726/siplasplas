#ifndef SIPLASPLAS_UTILITY_EXCEPTION_HPP
#define SIPLASPLAS_UTILITY_EXCEPTION_HPP

#include "error_logger.hpp"
#include <ctti/type_id.hpp>
#include <stdexcept>

namespace cpp
{

/**
 * \ingroup error-handling
 * \brief Logs an error message from an exception.
 *
 * This function logs an error report (See cpp::logError()) about a thrown
 * exception. The error report is of the form:
 *
 * ```
 * '<exception type>' exception thrown: <message>
 * ```
 *
 * \tparam Ex Type of the exception.
 * \param message Exception message.
 * \param framesToSkip Number of frames to skip from the backtrace. Default is cero. See cpp::logErrorSkippingFrames().
 *
 * ``` cpp
 * try
 * {
 *    throwingMan();
 * }
 * catch(const std::runtime_error& ex)
 * {
 *     logException<std::runtime_error>(ex.what());
 * }
 * ```
 */
template<typename Ex, typename Message>
void logException(const Message& message, std::size_t framesToSkip = 0)
{
    cpp::utility::logErrorSkippingFrames(framesToSkip + 1, "'{}' exception thrown: {}", ctti::type_id<Ex>().name(), message);
}

/**
 * \ingroup error-handling
 * \brief Logs an exception.
 *
 * This function logs a detailed error report from an exception object.
 * See cpp::logError(). The error report is of the form:
 * ```
 * '<exception type>' exception thrown: <exception.what()>
 * ```
 *
 * \param exception Exception object. The error message for the report is
 * get from `exception.what()`.
 * \param framesToSkip Number of frames to skip from the backtrace. Default is cero. See cpp::logErrorSkippingFrames().
 *
 * ``` cpp
 * try
 * {
 *    throwingMan();
 * }
 * catch(const std::runtime_error& ex)
 * {
 *     logException(ex);
 * }
 * ```
 */
template<typename Ex>
void logException(const Ex& exception, std::size_t framesToSkip = 0)
{
    logException<Ex>(exception.what(), framesToSkip + 1);
}

/**
 * \brief Declares a siplasplas exception type.
 *
 * This CRTP provides a convenient way to declare exception types
 * that follow siplasplas conventions. Siplasplas exceptions are automatically
 * logged to the error logger (See cpp::logException()) on construction.
 *
 * \tparam Ex Declared exception type. It's the derived class parameter of the CRTP.
 * \tparam Base Exception base class. std::exception by default.
 */
template<typename Ex, typename Base = std::exception>
class Exception : public Base
{
public:
    template<std::size_t N>
    Exception(const char (&message)[N]) :
        Base{message}
    {
        logException<Ex>(message);
    }

    Exception(const std::string& message) :
        Base{message}
    {
        logException<Ex>(message);
    }
};

/**
 * \ingroup error-handling
 * \brief Instances an exception with a custom error message.
 *
 * This function creates an exception object of the given type
 * using a custom parameterized error message (See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList)).
 * Exceptions instanced throught this function are reported automatically
 * (See cpp::logError() and cpp::logException()).
 *
 * \tparam Exception Exception type to instance.
 * \param framesToSkip Number of lower frames to skip from the report backtrace. See cpp::logErrorSkippingFrames().
 * \param message Exception error message. May have message argument placeholders. See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList).
 * \param args Error message arguments.
 *
 * \return A \p Exception instance with the specified error message.
 *
 * ``` cpp
 *
 * template<typename Function>
 * void businessLogic(Function callback)
 * {
 *     callback();
 * }
 *
 * int main()
 * {
 *     businessLogic([]
 *     {
 *         // throw an error skipping the callback frame from the report backtrace:
 *         throw exceptionSkippingFrames<std::runtime_error>(1, "Look, a runtime error!");
 *     });
 * }
 * ```
 */
template<typename Exception, typename... Args>
Exception exceptionSkippingFrames(std::size_t framesToSkip, const std::string& message, Args&&... args)
{
    auto formattedMessage = fmt::format(message, std::forward<Args>(args)...);

    // siplasplas exceptions are logged automatically during construction
    if(!std::is_base_of<::cpp::Exception<Exception>, Exception>::value)
    {
        // +1 to skip this frame too
        logException<Exception>(formattedMessage, framesToSkip + 1);
    }

    return Exception{formattedMessage};
}

template<typename Exception, typename... Args>
/**
 * \ingroup error-handling
 * \brief Instances an exception with a custom error message.
 *
 * This function creates an exception object of the given type
 * using a custom parameterized error message (See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList)).
 * Exceptions instanced throught this function are reported automatically
 * (See cpp::logError() and cpp::logException()).
 *
 * \tparam Exception Exception type to instance.
 * \param message Exception error message. May have message argument placeholders. See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList).
 * \param args Error message arguments.
 *
 * \return A \p Exception instance with the specified error message.
 *
 * ``` cpp
 *
 * template<typename Function>
 * void businessLogic(Function callback)
 * {
 *     callback();
 * }
 *
 * int main()
 * {
 *     businessLogic([]
 *     {
 *         // throw an error skipping the callback frame from the report backtrace:
 *         throw exceptionSkippingFrames<std::runtime_error>(1, "Look, a runtime error!");
 *     });
 * }
 * ```
 */
Exception exception(const std::string& message, Args&&... args)
{
    return exceptionSkippingFrames<Exception>(1, message, std::forward<Args>(args)...);
}

/**
 * \brief Throws an exception. See cpp::exception().
 */
template<typename Exception, typename... Args>
void Throw(const std::string& message, Args&&... args)
{
    throw cpp::exceptionSkippingFrames<Exception>(1, message, std::forward<Args>(args)...);
}

}

#endif // SIPLASPLAS_UTILITY_EXCEPTION_HPP
