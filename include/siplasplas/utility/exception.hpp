#ifndef SIPLASPLAS_UTILITY_EXCEPTION_HPP
#define SIPLASPLAS_UTILITY_EXCEPTION_HPP

#include <fmt/format.h>
#include <stdexcept>

namespace cpp
{

template<typename Exception, typename... Args>
Exception exception(const std::string& message, Args&&... args)
{
    return Exception{fmt::format(message, std::forward<Args>(args)...)};
}

template<typename Exception, typename... Args>
void Throw(const std::string& message, Args&&... args)
{
    throw cpp::exception<Exception>(message, std::forward<Args>(args)...);
}

}

#endif // SIPLASPLAS_UTILITY_EXCEPTION_HPP
