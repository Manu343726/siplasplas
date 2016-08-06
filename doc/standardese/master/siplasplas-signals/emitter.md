
---
layout: standardese-doc
---

# Header file `emitter.hpp`

``` cpp
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
    class SignalEmitter;
    
    template <typename Class>
    auto emit(const Class& object);
    
    template <typename Class>
    auto emit(Class& object);
}
```

## Class `cpp::SignalEmitter`<a id="cpp::SignalEmitter"></a>

``` cpp
class SignalEmitter
{
public:
    template <typename Caller, typename Function, typename R, typename Class, typename Args>
    static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*)(Args...) source, Function function);
    
    template <typename Caller, typename Callee, typename Function, typename R, typename Class, typename Args>
    static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*)(Args...) source, Callee& callee, Function function);
    
    template <typename Caller, typename Function, typename R, typename Class, typename Args>
    static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*)(Args...) source, Function function);
    
    template <typename Caller, typename Callee, typename Function, typename R, typename Class, typename Args>
    static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*)(Args...) source, Callee& callee, Function function);
    
    template <typename Caller, typename Callee, typename R, typename Args>
    static std::shared_ptr<const SignalSink> bypass(Caller& caller, R(Caller::*)(Args...) source, Callee& callee, R(Callee::*)(Args...) dest);
    
    template <typename Caller, typename Callee, typename R, typename Args>
    static std::shared_ptr<const SignalSink> bypass_async(Caller& caller, R(Caller::*)(Args...) source, Callee& callee, R(Callee::*)(Args...) dest);
    
    template <typename Class, typename R, typename FArgs, typename Args>
    static void emit(Class& emitter, R(Class::*)(FArgs...) function, Args&&... args);
    
    SignalEmitter() = default;
    
    SignalEmitter(SignalEmitter&&) = default;
    
    SignalEmitter& operator=(SignalEmitter&&) = default;
    
    ~SignalEmitter();
    
    void poll();
    
protected:
    template <typename Function, typename Args>
    void invoke(Function function, Args&&... args);
};
```

Class that can send and receive signals from other emitters

This class manages mapping from signals to sinks (i.e. outgoing connectiong from this object) as well as incomming connections. All connections are closed when the caller (source object) or the callee (destination object, if there is) are destroyed.

For details \\see signals

### Function template `cpp::SignalEmitter::connect<Caller, Function, R, Class, Args...>`<a id="cpp::SignalEmitter::connect<Caller, Function, R, Class, Args...>"></a>

``` cpp
template <typename Caller, typename Function, typename R, typename Class, typename Args>
static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*)(Args...) source, Function function);
```

Creates a direct connection from the given emitter object to the given function

Direct connections invoke the destination function just after the source signal is invoked. The destination function is invoked in the same thread the signal was emitted. This connections preserve caller affinity (The sink is invoked only if it was the caller object was who emitted the signal, not whenever any object emits the signal). Direct connections are implemented by registering instances of SyncSink on the caller object. The connection has no associated callee object, so there's no way to poll this connection except from the returned sink. Both source (The member function pointer representing the signal) and function must have the same signature.

#### Parameter `cpp::SignalEmitter::connect::caller`<a id="cpp::SignalEmitter::connect::caller"></a>

``` cpp
Caller& caller
```

object the signal is raised from. \\param Class::\*source Pointer to the class member function representing the signal. \\param function Function to be invoked when the signal is emitted.

-----

### Function template `cpp::SignalEmitter::connect<Caller, Callee, Function, R, Class, Args...>`<a id="cpp::SignalEmitter::connect<Caller, Callee, Function, R, Class, Args...>"></a>

``` cpp
template <typename Caller, typename Callee, typename Function, typename R, typename Class, typename Args>
static std::shared_ptr<const SignalSink> connect(Caller& caller, R(Class::*)(Args...) source, Callee& callee, Function function);
```

Creates a direct connection from the given emitter object to the given function, using an specific callee object.

Direct connections invoke the destination function just after the source signal is invoked. The destination function is invoked in the same thread the signal was emitted. This connections preserve caller affinity (The sink is invoked only if it was the caller object was who emitted the signal, not whenever any object emits the signal). Direct connections are implemented by registering instances of SyncSink on the caller object. The destination function has an associated callee object, so this connection could be polled directly from the callee object. If the destination function is a member function of the callee class, the function is invoked using callee. Both source (The member function pointer representing the signal) and function must have the same signature.

#### Parameter `cpp::SignalEmitter::connect::caller`<a id="cpp::SignalEmitter::connect::caller"></a>

``` cpp
Caller& caller
```

object the signal is raised from. \\param Class::\*source Pointer to the class member function representing the signal. \\param Callee& callee Destination object of the connection. \\param function Function to be invoked when the signal is emitted.

-----

### Function template `cpp::SignalEmitter::connect_async<Caller, Function, R, Class, Args...>`<a id="cpp::SignalEmitter::connect_async<Caller, Function, R, Class, Args...>"></a>

``` cpp
template <typename Caller, typename Function, typename R, typename Class, typename Args>
static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*)(Args...) source, Function function);
```

Creates an asynchronous connection between a signal and a function

Asynchronous connections are connections that don't directly invoke the destination function when the signal is invoked but delay the invocation until the user explicitly polls the connection. See SignalEmitter::poll() and SignalSink::poll(). Async connections are implemented by means of the AsyncSink class, which enqueues invocations on a thread-safe queue and deques and invokes the destination function when the connection is polled. **Async connections are designed for inter-thread communication**, where one thread acts as producer (invokes/emits the signal) and thread other consumes the connection (explicitly polls the connection). Please consider using direct connections by default and use async connections only as a communication bridge between two threads.

The destination function has no associated callee object, so the connection can be polled through the returned sink only. The destination function is executed in the same thread the connection is polled.

*Returns:* A shared pointer to the connection sink.

#### Parameter `cpp::SignalEmitter::connect_async::caller`<a id="cpp::SignalEmitter::connect_async::caller"></a>

``` cpp
Caller& caller
```

Object the signal is raised from. \\param Class::\*source Pointer to the class member representing the signal. \\param function Function to be invoked when the connection is polled (See details).

-----

### Function template `cpp::SignalEmitter::connect_async<Caller, Callee, Function, R, Class, Args...>`<a id="cpp::SignalEmitter::connect_async<Caller, Callee, Function, R, Class, Args...>"></a>

``` cpp
template <typename Caller, typename Callee, typename Function, typename R, typename Class, typename Args>
static std::shared_ptr<const SignalSink> connect_async(Caller& caller, R(Class::*)(Args...) source, Callee& callee, Function function);
```

Creates an asynchronous connection between a signal and a function using an specific callee object.

(For details on async connections in general, see SignalEmitter::connect\_async(caller, source, function) details).

This connection has an asociated callee object. SignalEmitter objects have a SignalEmitter::poll() function that polls all the connections comming to it (i.e. all connections which the object is acting as callee). The destination function is executed in the same thread the connection is polled.

*Returns:* A shared pointer to the connection sink.

#### Parameter `cpp::SignalEmitter::connect_async::caller`<a id="cpp::SignalEmitter::connect_async::caller"></a>

``` cpp
Caller& caller
```

Object the signal is raised from. \\param Class::\*source Pointer to the class member representing the signal. \\param function Function to be invoked when the connection is polled (See details).

-----

### Function template `cpp::SignalEmitter::bypass<Caller, Callee, R, Args...>`<a id="cpp::SignalEmitter::bypass<Caller, Callee, R, Args...>"></a>

``` cpp
template <typename Caller, typename Callee, typename R, typename Args>
static std::shared_ptr<const SignalSink> bypass(Caller& caller, R(Caller::*)(Args...) source, Callee& callee, R(Callee::*)(Args...) dest);
```

Connects two signals synchronously.

This connection bypasses signals so there's no need to have an intermediary sink to link one signal to another. The connection is synchronous, so when the source signal is emitted, the destination signal is emitted directly, in the same thread.

#### Parameter `cpp::SignalEmitter::bypass::caller`<a id="cpp::SignalEmitter::bypass::caller"></a>

``` cpp
Caller& caller
```

Emitter of the source signal. \\param Caller::\*source Source signal. \\param callee Emitter of the destination signal. Could be the caller too. \\param Callee::\*dest Destination signal.

-----

### Function template `cpp::SignalEmitter::bypass_async<Caller, Callee, R, Args...>`<a id="cpp::SignalEmitter::bypass_async<Caller, Callee, R, Args...>"></a>

``` cpp
template <typename Caller, typename Callee, typename R, typename Args>
static std::shared_ptr<const SignalSink> bypass_async(Caller& caller, R(Caller::*)(Args...) source, Callee& callee, R(Callee::*)(Args...) dest);
```

Connects two signals asynchronously.

This connection bypasses signals so there's no need to have an intermediary sink to link one signal to another. The destination signal is invoked whenever the callee object is polled. The destination signal is emitted from the thread the callee ofject is polled.

#### Parameter `cpp::SignalEmitter::bypass_async::caller`<a id="cpp::SignalEmitter::bypass_async::caller"></a>

``` cpp
Caller& caller
```

Emitter of the source signal. \\param Caller::\*source Source signal. \\param callee Emitter of the destination signal. Could be the caller too. \\param Callee::\*dest Destination signal.

-----

### Function template `cpp::SignalEmitter::emit<Class, R, FArgs..., Args...>`<a id="cpp::SignalEmitter::emit<Class, R, FArgs..., Args...>"></a>

``` cpp
template <typename Class, typename R, typename FArgs, typename Args>
static void emit(Class& emitter, R(Class::*)(FArgs...) function, Args&&... args);
```

Emits a signal on the given emitter

#### Parameter `cpp::SignalEmitter::emit::emitter`<a id="cpp::SignalEmitter::emit::emitter"></a>

``` cpp
Class& emitter
```

Object to emit the signal from. \\param Class::\*function Function pointer representing the signal. \\param args Signal arguments.

-----

### Function `cpp::SignalEmitter::poll`<a id="cpp::SignalEmitter::poll"></a>

``` cpp
void poll();
```

Polls this object for incomming signal emissions

For all connections coming to this SignalEmitter object, this method invokes SignalSink::poll() so all pending sygnals are resolved.

-----

-----

## Function template `cpp::emit<Class>`<a id="cpp::emit<Class>"></a>

``` cpp
template <typename Class>
auto emit(const Class& object);
```

Emits a const signal from the given const SignalEmitter

This function provides a succint way to emit signals from objects by providing a fake object to pick the required signal. Overloaded signals are supported.

*Returns:* A cpp::static\_reflection::Class\<Class\>::FakeObject instance which invokes the selected signal.

``` cpp
class Foo : public cpp::SignalEmitter
{
public:
void signal();
void signal(int i);
};

Foo foo;

cpp::emit(foo).signal(); // Emits &Foo::signal from foo
cpp::emit(foo).signal(42); // Emits &Foo::signal(int) with param '42' from Foo
```

### Template parameter `cpp::emit<Class>::Class`<a id="cpp::emit<Class>::Class"></a>

``` cpp
typename Class
```

Type of the signal emitter. **Static reflection metadata of this class should be available in the translation unit the emit() function is invoked**.

### Parameter `cpp::emit::object`<a id="cpp::emit::object"></a>

``` cpp
const Class& object
```

Object which emits the signal

-----

## Function template `cpp::emit<Class>`<a id="cpp::emit<Class>"></a>

``` cpp
template <typename Class>
auto emit(Class& object);
```

Emits a signal from the given SignalEmitter

This function provides a succint way to emit signals from objects by providing a fake object to pick the required signal. Overloaded signals are supported.

*Returns:* A cpp::static\_reflection::Class\<Class\>::FakeObject instance which invokes the selected signal.

``` cpp
class Foo : public cpp::SignalEmitter
{
public:
void signal();
void signal(int i);
};

Foo foo;

cpp::emit(foo).signal(); // Emits &Foo::signal from foo
cpp::emit(foo).signal(42); // Emits &Foo::signal(int) with param '42' from Foo
```

### Template parameter `cpp::emit<Class>::Class`<a id="cpp::emit<Class>::Class"></a>

``` cpp
typename Class
```

Type of the signal emitter. **Static reflection metadata of this class should be available in the translation unit the emit() function is invoked**.

### Parameter `cpp::emit::object`<a id="cpp::emit::object"></a>

``` cpp
Class& object
```

Object which emits the signal

-----
