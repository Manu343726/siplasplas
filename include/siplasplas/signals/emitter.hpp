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
#include <siplasplas/signals/export.hpp>

#include <memory>
#include <mutex>

/**
 * \defgroup signals
 *
 * \brief A simple multi-threaded message passing system.
 *
 * The `siplasplas-signals` module implements a simple message passing system where
 * objects raise *signals* in the form of member functions which cause connected
 * functions to be invoked.
 *
 * Despite other message passing systems, siplasplas does not have a class representation
 * for the *connection* concept, but represents connections as associations between `SignalEmitter`s
 * and SignalSinks. A signal sink is the destination of a raised signal, and keeps track of
 * the callee function (And or callee object).
 *
 * siplasplas-signals is not specifically designed with performance in mind but with
 * simplicity of user code:
 *
 *  - **No special tagging is needed**: siplasplas represents connections as associations
 *    between an emitter object, a class member function (the signal), and a destination
 *    function (A free function, a lambda, callee object + member function, etc).
 *    *"Signals"* are just an abstraction represented by member functions with an empty body.
 *
 *  - **Type erased connections**: Connections are type erased so there are two fixed cpp::SignalSink
 *    and cpp::SignalEmitter class hierarchies (As oppossed to Sink templates). This means the user just
 *    only has to inherit from cpp::SignalEmitter to raise and emmit signals, and SignalSink to
 *    implement a new category of Sink.
 *
 *  - **Explicit usage**: There are no implicit polling loops, the user is in charge of polling
 *    non-direct connections (Such as async connections between threads) for incoming signals.
 *
 *  \example signals/signals.cpp
 */

namespace cpp
{

/**
 * \ingroup signals
 *
 *
 * \brief Class that can send and receive signals from other emitters
 *
 * This class manages mapping from signals to sinks (i.e. outgoing connectiong
 * from this object) as well as incomming connections. All connections are closed
 * when the caller (source object) or the callee (destination object, if there is)
 * are destroyed.
 *
 * For details \see signals
 */
class SIPLASPLAS_SIGNALS_EXPORT SignalEmitter
{
public:

    /**
     * \brief Creates a direct connection from the given emitter object to the given function
     *
     * Direct connections invoke the destination function just after the source signal is invoked.
     * The destination function is invoked in the same thread the signal was emitted. This connections
     * preserve caller affinity (The sink is invoked only if it was the caller object was who emitted the signal,
     * not whenever any object emits the signal).
     * Direct connections are implemented by registering instances of SyncSink on the caller object.
     * The connection has no associated callee object, so there's no way to poll this connection except from
     * the returned sink.
     * Both source (The member function pointer representing the signal) and function must have the same signature.
     *
     * \param caller object the signal is raised from.
     * \param Class::*source Pointer to the class member function representing the signal.
     * \param function Function to be invoked when the signal is emitted.
     *
     * \return A shared pointer to the sink of the connection.
     */
    template<typename Caller, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*source)(Args...), Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new SyncSink{caller, function} };

        caller.registerConnection(source, sink);

        return sink;
    }

    /**
     * \brief Creates a direct connection from the given emitter object to the given function,
     * using an specific callee object.
     *
     * Direct connections invoke the destination function just after the source signal is invoked.
     * The destination function is invoked in the same thread the signal was emitted. This connections
     * preserve caller affinity (The sink is invoked only if it was the caller object was who emitted the signal,
     * not whenever any object emits the signal).
     * Direct connections are implemented by registering instances of SyncSink on the caller object.
     * The destination function has an associated callee object, so this connection could be polled directly from the callee
     * object. If the destination function is a member function of the callee class, the function is invoked using callee.
     * Both source (The member function pointer representing the signal) and function must have the same signature.
     *
     * \param caller object the signal is raised from.
     * \param Class::*source Pointer to the class member function representing the signal.
     * \param Callee& callee Destination object of the connection.
     * \param function Function to be invoked when the signal is emitted.
     *
     * \return A shared pointer to the sink of the connection.
     */
    template<typename Caller, typename Callee, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*source)(Args...), Callee& callee, Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new SyncSink{caller, callee, function} };

        caller.registerConnection(source, sink);
        callee.registerIncommingConnection(sink);

        return sink;
    }

    /**
     * \brief Creates an asynchronous connection between a signal and a function
     *
     * Asynchronous connections are connections that don't directly invoke the destination function when the signal is invoked
     * but delay the invocation until the user explicitly polls the connection. See SignalEmitter::poll() and SignalSink::poll().
     * Async connections are implemented by means of the AsyncSink class, which enqueues invocations on a thread-safe queue and deques
     * and invokes the destination function when the connection is polled. **Async connections are designed for inter-thread communication**,
     * where one thread acts as producer (invokes/emits the signal) and thread other consumes the connection (explicitly polls the connection).
     * Please consider using direct connections by default and use async connections only as a communication bridge between two threads.
     *
     * The destination function has no associated callee object, so the connection can be polled through the returned sink only.
     * The destination function is executed in the same thread the connection is polled.
     *
     * \param caller Object the signal is raised from.
     * \param Class::*source Pointer to the class member representing the signal.
     * \param function Function to be invoked when the connection is polled (See details).
     *
     * \returns A shared pointer to the connection sink.
     */
    template<typename Caller, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*source)(Args...), Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new AsyncSink{caller, function} };

        caller.registerConnection(source, sink);

        return sink;
    }

    /**
     * \brief Creates an asynchronous connection between a signal and a function
     * using an specific callee object.
     *
     * (For details on async connections in general, see SignalEmitter::connect_async(caller, source, function)
     * details).
     *
     * This connection has an asociated callee object. SignalEmitter objects have a SignalEmitter::poll() function
     * that polls all the connections comming to it (i.e. all connections which the object is acting as callee).
     * The destination function is executed in the same thread the connection is polled.
     *
     * \param caller Object the signal is raised from.
     * \param Class::*source Pointer to the class member representing the signal.
     * \param function Function to be invoked when the connection is polled (See details).
     *
     * \returns A shared pointer to the connection sink.
     */
    template<typename Caller, typename Callee, typename Function, typename R, typename Class, typename... Args>
    static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*source)(Args...), Callee& callee, Function function)
    {
        static_assert(equal_signature<decltype(source), Function>::value, "Signal vs slot signatures don't match");
        std::shared_ptr<SignalSink> sink{ new AsyncSink{caller, callee, function} };

        caller.registerConnection(source, sink);
        callee.registerIncommingConnection(sink);

        return sink;
    }

    /**
     * \brief Connects two signals synchronously.
     *
     * This connection bypasses signals so there's no need to have an intermediary
     * sink to link one signal to another. The connection is synchronous, so when
     * the source signal is emitted, the destination signal is emitted directly, in
     * the same thread.
     *
     * \param caller Emitter of the source signal.
     * \param Caller::*source Source signal.
     * \param callee Emitter of the destination signal. Could be the caller too.
     * \param Callee::*dest Destination signal.
     *
     * \return A shared pointer to the SYncSink implementing the connection.
     */
    template<typename Caller, typename Callee, typename R, typename... Args>
    static std::shared_ptr<const SignalSink> bypass(Caller& caller, R(Caller::*source)(Args...), Callee& callee, R(Callee::*dest)(Args...))
    {
        static_assert(equal_signature<decltype(source), decltype(dest)>::value, "Signal vs slot signatures don't match");
        return connect(caller, source, callee, [&callee, dest](Args... args)
        {
            emit(callee, dest, args...);
        });
    }

    /**
     * \brief Connects two signals asynchronously.
     *
     * This connection bypasses signals so there's no need to have an intermediary
     * sink to link one signal to another. The destination signal is invoked whenever the
     * callee object is polled. The destination signal is emitted from the thread the callee
     * ofject is polled.
     *
     * \param caller Emitter of the source signal.
     * \param Caller::*source Source signal.
     * \param callee Emitter of the destination signal. Could be the caller too.
     * \param Callee::*dest Destination signal.
     *
     * \return A shared pointer to the SYncSink implementing the connection.
     */
    template<typename Caller, typename Callee, typename R, typename... Args>
    static std::shared_ptr<const SignalSink> bypass_async(Caller& caller, R(Caller::*source)(Args...), Callee& callee, R(Callee::*dest)(Args...))
    {
        static_assert(equal_signature<decltype(source), decltype(dest)>::value, "Signal vs slot signatures don't match");
        return connect_async(caller, source, callee, [&callee, dest](Args... args)
        {
            emit(callee, dest, args...);
        });
    }

    /**
     * \brief Emits a signal on the given emitter
     *
     * \param emitter Object to emit the signal from.
     * \param Class::*function Function pointer representing the signal.
     * \param args Signal arguments.
     */
    template<typename Class, typename R, typename... FArgs, typename... Args>
    static void emit(Class& emitter, R(Class::*function)(FArgs...), Args&&... args)
    {
        emitter.invoke(function, std::forward<Args>(args)...);
    }

    SignalEmitter() = default;
    SignalEmitter(SignalEmitter&&) = default;
    SignalEmitter& operator=(SignalEmitter&&) = default;
    ~SignalEmitter();

    /**
     * \brief Polls this object for incomming signal emissions
     *
     * For all connections coming to this SignalEmitter object, this method
     * invokes SignalSink::poll() so all pending sygnals are resolved.
     */
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

/**
 * \ingroup signals
 *
 *
 * \brief Emits a const signal from the given const SignalEmitter
 *
 * This function provides a succint way to emit signals from objects by providing a
 * fake object to pick the required signal. Overloaded signals are supported.
 *
 * \tparam Class Type of the signal emitter. **Static reflection metadata of this class
 * should be available in the translation unit the emit() function is invoked**.
 *
 * \param object Object which emits the signal
 *
 * \returns A cpp::static_reflection::Class<Class>::FakeObject instance which invokes the
 * selected signal.
 *
 * ``` cpp
 * class Foo : public cpp::SignalEmitter
 * {
 * public:
 *     void signal();
 *     void signal(int i);
 * };
 *
 * Foo foo;
 *
 * cpp::emit(foo).signal(); // Emits &Foo::signal from foo
 * cpp::emit(foo).signal(42); // Emits &Foo::signal(int) with param '42' from Foo
 * ```
 */
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

/**
 * \ingroup signals
 *
 *
 * \brief Emits a signal from the given SignalEmitter
 *
 * This function provides a succint way to emit signals from objects by providing a
 * fake object to pick the required signal. Overloaded signals are supported.
 *
 * \tparam Class Type of the signal emitter. **Static reflection metadata of this class
 * should be available in the translation unit the emit() function is invoked**.
 *
 * \param object Object which emits the signal
 *
 * \returns A cpp::static_reflection::Class<Class>::FakeObject instance which invokes the
 * selected signal.
 *
 * ``` cpp
 * class Foo : public cpp::SignalEmitter
 * {
 * public:
 *     void signal();
 *     void signal(int i);
 * };
 *
 * Foo foo;
 *
 * cpp::emit(foo).signal(); // Emits &Foo::signal from foo
 * cpp::emit(foo).signal(42); // Emits &Foo::signal(int) with param '42' from Foo
 * ```
 */
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
