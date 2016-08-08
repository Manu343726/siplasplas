---
layout: standardese-doc
---

# Header file `exception.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_EXCEPTION_HPP 

#include "error_logger.hpp"

#include <ctti/type_id.hpp>

#include <stdexcept>

namespace cpp
{
    template <typename Ex, typename Message>
    void logException(const Message& message, std::size_t framesToSkip = 0);
    
    template <typename Ex>
    void logException(const Ex& exception, std::size_t framesToSkip = 0);
    
    template <typename Ex, typename Base = std::exception>
    class Exception;
    
    template <typename Exception, typename ... Args>
    Exception exceptionSkippingFrames(std::size_t framesToSkip, const std::string& message, Args&&... args);
    
    template <typename Exception, typename ... Args>
    Exception exception(const std::string& message, Args&&... args);
    
    template <typename Exception, typename ... Args>
    void Throw(const std::string& message, Args&&... args);
}
```

## Function template `cpp::logException<Ex, Message>`<a id="cpp::logException<Ex, Message>"></a>

``` cpp
template <typename Ex, typename Message>
void logException(const Message& message, std::size_t framesToSkip = 0);
```

This function logs an error report (See cpp::logError()) about a thrown exception. The error report is of the form:

    '<exception type>' exception thrown: <message>

``` cpp
try
{
   throwingMan();
}
catch(const std::runtime_error& ex)
{
    logException<std::runtime_error>(ex.what());
}
```

### Template parameter `cpp::logException<Ex, Message>::Ex`<a id="cpp::logException<Ex, Message>::Ex"></a>

``` cpp
typename Ex
```

Type of the exception. \\param message Exception message. \\param framesToSkip Number of frames to skip from the backtrace. Default is cero. See cpp::logErrorSkippingFrames().

-----

## Function template `cpp::logException<Ex>`<a id="cpp::logException<Ex>"></a>

``` cpp
template <typename Ex>
void logException(const Ex& exception, std::size_t framesToSkip = 0);
```

This function logs a detailed error report from an exception object. See cpp::logError(). The error report is of the form:

    '<exception type>' exception thrown: <exception.what()>

``` cpp
try
{
   throwingMan();
}
catch(const std::runtime_error& ex)
{
    logException(ex);
}
```

### Parameter `cpp::logException::exception`<a id="cpp::logException::exception"></a>

``` cpp
const Ex& exception
```

Exception object. The error message for the report is get from `exception.what()`. \\param framesToSkip Number of frames to skip from the backtrace. Default is cero. See cpp::logErrorSkippingFrames().

-----

## Class template `cpp::Exception<Ex, Base>`<a id="cpp::Exception<Ex, Base>"></a>

``` cpp
template <typename Ex, typename Base = std::exception>
class Exception
: public Base
{
public:
    template <std::size_t N>
    Exception(const char (&) message[N]);
    
    Exception(const std::string& message);
};
```

Declares a siplasplas exception type.

This CRTP provides a convenient way to declare exception types that follow siplasplas conventions. Siplasplas exceptions are automatically logged to the error logger (See cpp::logException()) on construction.

### Template parameter `cpp::Exception<Ex, Base>::Ex`<a id="cpp::Exception<Ex, Base>::Ex"></a>

``` cpp
typename Ex
```

Declared exception type. It's the derived class parameter of the CRTP. \\tparam Base Exception base class. std::exception by default.

-----

## Function template `cpp::exceptionSkippingFrames<Exception, Args...>`<a id="cpp::exceptionSkippingFrames<Exception, Args...>"></a>

``` cpp
template <typename Exception, typename ... Args>
Exception exceptionSkippingFrames(std::size_t framesToSkip, const std::string& message, Args&&... args);
```

This function creates an exception object of the given type using a custom parameterized error message (See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList)). Exceptions instanced throught this function are reported automatically (See cpp::logError() and cpp::logException()).

``` cpp

template<typename Function>
void businessLogic(Function callback)
{
    callback();
}

int main()
{
    businessLogic([]
    {
        // throw an error skipping the callback frame from the report backtrace:
        throw exceptionSkippingFrames<std::runtime_error>(1, "Look, a runtime error!");
    });
}
```

### Template parameter `cpp::exceptionSkippingFrames<Exception, Args...>::Exception`<a id="cpp::exceptionSkippingFrames<Exception, Args...>::Exception"></a>

``` cpp
typename Exception
```

Exception type to instance. \\param framesToSkip Number of lower frames to skip from the report backtrace. See cpp::logErrorSkippingFrames(). \\param message Exception error message. May have message argument placeholders. See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList). \\param args Error message arguments.

-----

## Function template `cpp::Throw<Exception, Args...>`<a id="cpp::Throw<Exception, Args...>"></a>

``` cpp
template <typename Exception, typename ... Args>
void Throw(const std::string& message, Args&&... args);
```

Throws an exception. See cpp::exception().

-----
