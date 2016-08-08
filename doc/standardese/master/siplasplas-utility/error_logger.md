---
layout: standardese-doc
---

# Header file `error_logger.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_ERRORLOGGER_HPP 

#include <siplasplas/logger/logger.hpp>

#include <backward-cpp/backward.hpp>

#include <fmt/format.h>

#include <siplasplas/utility/export.hpp>

namespace cpp
{
    namespace utility
    {
         spdlog::logger& errorLogger();
        
        template <typename Message, typename ... Args>
        void logErrorSkippingFrames(std::size_t framesToSkip, const Message& message, Args&&... args);
        
        template <typename Message, typename ... Args>
        void logError(const Message& message, Args&&... args);
    }
}
```

## Function `cpp::utility::errorLogger`<a id="cpp::utility::errorLogger"></a>

``` cpp
 spdlog::logger& errorLogger();
```

This function returns a logger pointing to an isolated siplasplas errors log file. All exceptions and assertion failures are logged throught this logger.

@return Reference to the spdlog logger.

-----

## Function template `cpp::utility::logErrorSkippingFrames<Message, Args...>`<a id="cpp::utility::logErrorSkippingFrames<Message, Args...>"></a>

``` cpp
template <typename Message, typename ... Args>
void logErrorSkippingFrames(std::size_t framesToSkip, const Message& message, Args&&... args);
```

This function logs a detailed error report with a stack trace ending at the caller function. Also supports skipping the last \\p framesToSkip stack frames from the trace in case the trace is invoked from an API with multiple implementation layers. The generated report follows the format:

    #i "<binary file>", at <address> in <function>
    #i-1 "<binary file>", at <address> in <function>
    ...
    #1 "<binary file>", at <address> in <function>
    #0 "<binary file>", at <address> in <function>
    
    <error message>

where the lower (of index 0) line in the backtrace is the latest (caller) stack frame. The error message is logged with critical level and the backtrace with debug level. The backtrace supports a max of 32 frames. If the backtrace is longer, uppermost frames will be ignored.

``` cpp
void justBecauseExceptionsAreSlow(int i)
{
   // Because we miss nodejs, let's callback everything!
   auto callback = []
   {
       if(i != 42)
       {
           // Log an error skipping the callback frame:
           cpp::logErrorSkippingFrames(1, "Error: {} is not a valid answer Deep Thought!", i);
       }
   };

   callback();
}
```

### Parameter `cpp::utility::logErrorSkippingFrames::framesToSkip`<a id="cpp::utility::logErrorSkippingFrames::framesToSkip"></a>

``` cpp
std::size_t framesToSkip
```

Number of lower frames to skip from the backtrace. \\param message Error message. May contain argument placeholders. See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList). \\param args Error message arguments.

-----

## Function template `cpp::utility::logError<Message, Args...>`<a id="cpp::utility::logError<Message, Args...>"></a>

``` cpp
template <typename Message, typename ... Args>
void logError(const Message& message, Args&&... args);
```

Effectively calls `logErrorSkippingFrames(1, message, args...)`.

``` cpp
logError("unexpected error! (errno={}", errno);
```

### Parameter `cpp::utility::logError::message`<a id="cpp::utility::logError::message"></a>

``` cpp
const Message& message
```

Error message. May contain argument placeholders. \\param args Error message arguments.

-----
