#ifndef SIPLASPLAS_SIGNALS_EMITTER_HPP
#define SIPLASPLAS_SIGNALS_EMITTER_HPP

#include <siplasplas/reflection/dynamic/object_manip.hpp>
#include <siplasplas/reflection/dynamic/function_pointer.hpp>
#include <siplasplas/utility/hash.hpp>
#include <siplasplas/reflection/api.hpp>
#include "syncsink.hpp"
#include "asyncsink.hpp"

#include <memory>
#include <mutex>

namespace cpp
{

class SignalEmitter
{
public:
    template<typename Function, typename R, typename Class, typename... Args>
    static void connect(SignalEmitter& caller, R(Class::*source)(Args...), Function function)
    {
        std::shared_ptr<SignalSink> sink{ new SyncSink{caller, function} };

        caller.registerConnection(source, sink);
    }

    template<typename Function, typename R, typename Class, typename... Args>
    static void connect(SignalEmitter& caller, R(Class::*source)(Args...), SignalEmitter& callee, Function function)
    {
        std::shared_ptr<SignalSink> sink{ new SyncSink{caller, callee, function} };

        caller.registerConnection(source, sink);
        callee.registerIncommingConnection(sink);
    }

    template<typename Function, typename R, typename Class, typename... Args>
    static void connect_async(SignalEmitter& caller, R(Class::*source)(Args...), Function function)
    {
        std::shared_ptr<SignalSink> sink{ new AsyncSink{caller, function} };

        caller.registerConnection(source, sink);
    }

    template<typename Function, typename R, typename Class, typename... Args>
    static void connect_async(SignalEmitter& caller, R(Class::*source)(Args...), SignalEmitter& callee, Function function)
    {
        std::shared_ptr<SignalSink> sink{ new AsyncSink{caller, callee, function} };

        caller.registerConnection(source, sink);
        callee.registerIncommingConnection(sink);
    }


    template<typename Class, typename R, typename... FArgs, typename... Args>
    static void emit(Class& emitter, R(Class::*function)(FArgs...), Args&&... args)
    {
        emitter.invoke(function, std::forward<Args>(args)...);
    }


    ~SignalEmitter()
    {
        for(auto& sink : _incomingConnections)
        {
            sink->caller()->disconnectCallee(this);
        }
    }

    void poll()
    {
        for(auto& sink : _incomingConnections)
        {
            sink->pull();
        }
    }

protected:
    template<typename Function, typename... Args>
    void invoke(Function function, Args&&... args)
    {
        auto fptr = reinterpret_cast<void*>(function);
        auto it = _connections.find(fptr);

        if(it != _connections.end())
        {
            auto& sinks = it->second;

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
    cpp::HashTable<void*, std::vector<std::shared_ptr<SignalSink>>> _connections;
    std::mutex _lockConnections;
    std::mutex _lockIncommingConnections;

    void disconnectCallee(SignalEmitter* callee)
    {
        std::lock_guard<std::mutex> guard{_lockConnections};

        for(auto& keyValue : _connections)
        {
            auto& sinks = keyValue.second;

            sinks.erase(std::remove_if(sinks.begin(), sinks.end(), [callee](const std::shared_ptr<SignalSink>& sink)
            {
                return sink->callee() == callee;
            }), sinks.end());
        }
    }

    template<typename Function>
    void registerConnection(Function function, const std::shared_ptr<SignalSink>& sink)
    {
        std::lock_guard<std::mutex> guard{_lockConnections};
        auto fptr = reinterpret_cast<void*>(function);

        _connections[fptr].push_back(sink);
    }

    void registerIncommingConnection(const std::shared_ptr<SignalSink>& sink)
    {
        std::lock_guard<std::mutex> guard{_lockIncommingConnections};
        _incomingConnections.insert(sink);
    }
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
