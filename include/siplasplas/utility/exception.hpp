#ifndef SIPLASPLAS_UTILITY_EXCEPTION_HPP
#define SIPLASPLAS_UTILITY_EXCEPTION_HPP

#include "error_logger.hpp"
#include <fmt/format.h>
#include <ctti/type_id.hpp>
#include <stdexcept>

namespace cpp
{

template<typename Ex, typename Message>
void logException(const Message& message, std::size_t framesToSkip = 0)
{
    cpp::utility::logErrorSkippingFrames(framesToSkip + 1, "'{}' exception thrown: {}", ctti::type_id<Ex>().name(), message);
}

template<typename Ex>
void logException(const Ex& exception, std::size_t framesToSkip = 0)
{
    logException<Ex>(exception.what(), framesToSkip + 1);
}

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
Exception exception(const std::string& message, Args&&... args)
{
    return exceptionSkippingFrames<Exception>(1, message, std::forward<Args>(args)...);
}

template<typename Exception, typename... Args>
void Throw(const std::string& message, Args&&... args)
{
    throw cpp::exceptionSkippingFrames<Exception>(1, message, std::forward<Args>(args)...);
}

}

#endif // SIPLASPLAS_UTILITY_EXCEPTION_HPP
