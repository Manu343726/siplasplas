#ifndef SIPLASPLAS_UTILITY_ERRORLOGGER_HPP
#define SIPLASPLAS_UTILITY_ERRORLOGGER_HPP

#include <siplasplas/logger/logger.hpp>
#include <backward/backward.hpp>
#include <fmt/format.h>
#include <siplasplas/utility/export.hpp>

/**
 * \defgroup error-handling
 * \ingroup utility
 * \brief Error handling tools.
 *
 * This submodule contains tools to handle errors in different
 * ways and get detailed error reports.
 *
 * \todo cpp::Expect template for "fast" error handling.
 */

namespace cpp
{

namespace utility
{

/**
 * \ingroup error-handling
 * \brief Isolated logger for detailed error logging.
 *
 * This function returns a logger pointing to an isolated siplasplas
 * errors log file. All exceptions and assertion failures are logged
 * throught this logger.
 *
 * @return Reference to the spdlog logger.
 */
SIPLASPLAS_UTILITY_EXPORT spdlog::logger& errorLogger();

/**
 * \ingroup error-handling
 * \brief Log an error with its backtrace. Also skip the latest \p framesToSkip stack frames from stack trace.
 *
 * This function logs a detailed error report with a stack trace ending at the caller function.
 * Also supports skipping the last \p framesToSkip stack frames from the trace in case the
 * trace is invoked from an API with multiple implementation layers.
 * The generated report follows the format:
 *
 * ```
 * #i "<binary file>", at <address> in <function>
 * #i-1 "<binary file>", at <address> in <function>
 * ...
 * #1 "<binary file>", at <address> in <function>
 * #0 "<binary file>", at <address> in <function>
 *
 * <error message>
 * ```
 *
 * where the lower (of index 0) line in the backtrace is the latest (caller) stack frame.
 * The error message is logged with critical level and the backtrace with debug level.
 * The backtrace supports a max of 32 frames. If the backtrace is longer, uppermost frames
 * will be ignored.
 *
 * \param framesToSkip Number of lower frames to skip from the backtrace.
 * \param message Error message. May contain argument placeholders. See [`fmt::format()`](http://fmtlib.net/latest/api.html#_CPPv2N3fmt6formatE10CStringRef7ArgList).
 * \param args Error message arguments.
 *
 * ``` cpp
 * void justBecauseExceptionsAreSlow(int i)
 * {
 *    // Because we miss nodejs, let's callback everything!
 *    auto callback = []
 *    {
 *        if(i != 42)
 *        {
 *            // Log an error skipping the callback frame:
 *            cpp::logErrorSkippingFrames(1, "Error: {} is not a valid answer Deep Thought!", i);
 *        }
 *    };
 *
 *    callback();
 * }
 * ```
 */
template<typename Message, typename... Args>
void logErrorSkippingFrames(std::size_t framesToSkip, const Message& message, Args&&... args)
{
    using namespace backward;
    static constexpr std::size_t MAX_FRAMES = 32;
    StackTrace stackTrace;
    TraceResolver traceResolver;
    stackTrace.load_here(MAX_FRAMES);
    traceResolver.load_stacktrace(stackTrace);

    const std::size_t FRAMES_COUNT = stackTrace.size();

    framesToSkip += 3; // Skip this frame and the backward-cpp implementation ones
                       // (On linux these are backward::StackTraceImpl<Linux>::load_here()
                       // plus backtrace::detail::unwind())

    for(std::size_t i = 0; i < FRAMES_COUNT - framesToSkip; ++i)
    {
        const auto& trace = traceResolver.resolve(stackTrace[FRAMES_COUNT - i - 1]);

        errorLogger().debug(
            "#{}  \"{}\", at {}, in {}",
            FRAMES_COUNT - framesToSkip - i,
            trace.object_filename,
            trace.addr,
            trace.object_function
        );

    }

    errorLogger().critical(fmt::format(message, std::forward<Args>(args)...));
}

/**
 * \ingroup error-handling
 * \brief Logs an error with its backtrace. See cpp::logErrorSkippingFrames().
 *
 * Effectively calls `logErrorSkippingFrames(1, message, args...)`.
 *
 * \param message Error message. May contain argument placeholders.
 * \param args Error message arguments.
 *
 * ``` cpp
 * logError("unexpected error! (errno={}", errno);
 * ```
 */
template<typename Message, typename... Args>
void logError(const Message& message, Args&&... args)
{
    logErrorSkippingFrames(1, message, std::forward<Args>(args)...);
}

}

}

#endif // SIPLASPLAS_UTILITY_ERRORLOGGER_HPP
