#ifndef SIPLASPLAS_UTILITY_ERRORLOGGER_HPP
#define SIPLASPLAS_UTILITY_ERRORLOGGER_HPP

#include <siplasplas/logger/logger.hpp>
#include <backward-cpp/backward.hpp>
#include <fmt/format.h>
#include <siplasplas/utility/export.hpp>

namespace cpp
{

namespace utility
{

SIPLASPLAS_UTILITY_EXPORT spdlog::logger& errorLogger();

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

template<typename Message, typename... Args>
void logError(const Message& message, Args&&... args)
{
    logErrorSkippingFrames(1, message, std::forward<Args>(args)...);
}

}

}

#endif // SIPLASPLAS_UTILITY_ERRORLOGGER_HPP
