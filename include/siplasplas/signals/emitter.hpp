#ifndef SIPLASPLAS_SIGNALS_EMITTER_HPP
#define SIPLASPLAS_SIGNALS_EMITTER_HPP

#include <siplasplas/reflection/dynamic/object_manip.hpp>
#include <siplasplas/reflection/dynamic/function_pointer.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <siplasplas/utility/hash.hpp>
#include <siplasplas/reflection/api.hpp>
#include "syncsink.hpp"
#include "asyncsink.hpp"
#include "logger.hpp"

#include <memory>
#include <mutex>

namespace cpp
{

class SignalEmitter
{
public:
    template<typename Caller, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*source)(Args...), Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new SyncSink{caller, function} };

        caller.registerConnection(source, sink);

        return sink;
    }

    template<typename Caller, typename Callee, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*source)(Args...), Callee& callee, Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new SyncSink{caller, callee, function} };

        caller.registerConnection(source, sink);
        callee.registerIncommingConnection(sink);

        return sink;
    }

    template<typename Caller, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*source)(Args...), Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new AsyncSink{caller, function} };

        caller.registerConnection(source, sink);

        return sink;
    }

    template<typename Caller, typename Callee, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*source)(Args...), Callee& callee, Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new AsyncSink{caller, callee, function} };

        caller.registerConnection(source, sink);
        callee.registerIncommingConnection(sink);

        return sink;
    }

    template<typename Caller, typename Callee, typename R, typename... Args>
    static std::shared_ptr<const SignalSink> bypass(Caller& caller, R(Caller::*source)(Args...), Callee& callee, R(Callee::*dest)(Args...))
    {
        static_assert(equal_signature<decltype(source), decltype(dest)>::value, "Signal vs slot signatures don't match");
        return connect(caller, source, callee, [&callee, dest](Args... args)
        {
            emit(callee, dest, args...);
        });
    }

    template<typename Caller, typename Callee, typename R, typename... Args>
    static std::shared_ptr<const SignalSink> bypass_async(Caller& caller, R(Caller::*source)(Args...), Callee& callee, R(Callee::*dest)(Args...))
    {
        static_assert(equal_signature<decltype(source), decltype(dest)>::value, "Signal vs slot signatures don't match");
        return connect_async(caller, source, callee, [&callee, dest](Args... args)
        {
            emit(callee, dest, args...);
        });
    }

    template<typename Class, typename R, typename... FArgs, typename... Args>
    static void emit(Class& emitter, R(Class::*function)(FArgs...), Args&&... args)
    {
        emitter.invoke(function, std::forward<Args>(args)...);
    }

    SignalEmitter() = default;
    SignalEmitter(SignalEmitter&&) = default;
    SignalEmitter& operator=(SignalEmitter&&) = default;
    ~SignalEmitter();

    void poll();

protected:
    template<typename Function, typename... Args>
    void invoke(Function function, Args&&... args)
    {
        auto it = _connections.find(::cpp::hash(function));

        if(it != _connections.end())
        {
            auto& sinks = it->second;

#ifdef SIPLASPLAS_LOG_SIGNALS
            {
                static ctti::type_id_t argsTypes[] = {ctti::type_id<decltype(std::forward<Args>(args))>()..., ctti::type_id<void>()};
                cpp::dynamic_reflection::Object objectArgs[] = {std::forward<Args>(args)..., cpp::dynamic_reflection::Object()};

                signals::log().debug("Emitting signal from @{}. Args (count={})", static_cast<void*>(this), sizeof...(args));

                for(std::size_t i = 0; i < sizeof...(Args); ++i)
                {
                    signals::log().debug(
                        "  Arg({}): value '{}', type '{}'",
                        i,
                        objectArgs[i].toString(),
                        argsTypes[i].name()
                    );
                }
            }
#endif
            for(auto& sink : sinks)
            {
                (*sink)(
                    std::forward<Args>(args)...
                );
            }
        }
    }

private:
    cpp::HashSet<std::shared_ptr<SignalSink>> _incomingConnections;
    cpp::HashTable<std::size_t, std::vector<std::shared_ptr<SignalSink>>> _connections;
    std::mutex _lockConnections;
    std::mutex _lockIncommingConnections;

    void disconnectCallee(SignalEmitter* callee);

    template<typename Function>
    void registerConnection(Function function, const std::shared_ptr<SignalSink>& sink)
    {
        std::lock_guard<std::mutex> guard{_lockConnections};
        _connections[::cpp::hash(function)].push_back(sink);
    }

    void registerIncommingConnection(const std::shared_ptr<SignalSink>& sink);
};

template<typename Class>
auto emit(const Class& object)
{
    return ::cpp::static_reflection::Class<Class>::fakeObject(
        object,
        [](const Class& object, auto method, auto&&... args)
        {
            SignalEmitter::emit(object, method, std::forward<decltype(args)>(args)...);
        }
    );
}

template<typename Class>
auto emit(Class& object)
{
    return ::cpp::static_reflection::Class<Class>::fakeObject(
        object,
        [](Class& object, auto method, auto&&... args)
        {
            SignalEmitter::emit(object, method, std::forward<decltype(args)>(args)...);
        }
    );
}


}

#endif // SIPLASPLAS_SIGNALS_EMITTER_HPP
