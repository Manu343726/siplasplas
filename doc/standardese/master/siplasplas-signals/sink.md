---
layout: standardese-doc
---

# Header file `sink.hpp`

``` cpp
#define SIPLASPLAS_SIGNALS_SINK_HPP 

#include <siplasplas/reflection/dynamic/object_manip.hpp>

#include <siplasplas/signals/export.hpp>

#include <type_traits>

namespace cpp
{
    class SignalSink;
}
```

## Class `cpp::SignalSink`<a id="cpp::SignalSink"></a>

``` cpp
class SignalSink
{
public:
    template <typename Caller, typename Callee>
    SignalSink(Caller& caller, Callee& callee);
    
    template <typename Caller>
    SignalSink(Caller& caller);
    
    virtual ~SignalSink() = default;
    
    template <typename ... Args>
    void operator()(Args&&... args);
    
    SignalEmitter* callee() const;
    
    SignalEmitter* caller() const;
    
    virtual bool pull() = 0
    
protected:
    virtual void invoke() = 0
    
    virtual bool invokeWithoutCallee() const = 0
};
```

Interface to the signals sink API

A signal sink represents the destination of an emitted signal, that is, the action that should be invoked and how. Sinks keep track of the objects participating in a signal interchange (The source SignalEmitter and the destination object, if any). The interface provides two member functions to be overrided:

  - **SignalSink::invoke()**: Implements the behavior of the sink, that is, what is done when the source signal is emitted.

  - **SignalSink::pull()**: Implements the pull behavior of the sink, what should be done if the user of the connection explicitly asks for incoming signals.

Also the user must specify whether the sink should be invoked with ot without a callee object (See SignalSink::operator()() template). **Callee (if any) is always the first argument of type erased invoke() function**.

When an user invokes the signal, he/she does through the SignalSInk::invoke() template, which perfect forwards the signal arguments to the type erased protected functions above.

### Function template `cpp::SignalSink::SignalSink<Caller, Callee>`<a id="cpp::SignalSink::SignalSink<Caller, Callee>"></a>

``` cpp
template <typename Caller, typename Callee>
SignalSink(Caller& caller, Callee& callee);
```

Constructs a sink given the connection caller and the callee

This constructor is used by child classes to initialize connection participants. Note the SignalSink is pure virtual.

#### Parameter `cpp::SignalSink::SignalSink::caller`<a id="cpp::SignalSink::SignalSink::caller"></a>

``` cpp
Caller& caller
```

Reference to the caller object. Caller must inherit SignalEmitter. \\param callee Reference to the callee object. Caller must inherit SignalEmitter.

-----

### Function template `cpp::SignalSink::SignalSink<Caller>`<a id="cpp::SignalSink::SignalSink<Caller>"></a>

``` cpp
template <typename Caller>
SignalSink(Caller& caller);
```

Constructs a sink given the connection caller object.

This constructor is used by child classes to initialize connection caller. This constructor is used by connections with no associated callee (Such as connections invoking a lambda or a free function). Note the SignalSink is pure virtual.

#### Parameter `cpp::SignalSink::SignalSink::caller`<a id="cpp::SignalSink::SignalSink::caller"></a>

``` cpp
Caller& caller
```

Reference to the caller object. Caller must inherit SignalEmitter.

-----

### Function template `cpp::SignalSink::operator()<Args...>`<a id="cpp::SignalSink::operator()<Args...>"></a>

``` cpp
template <typename ... Args>
void operator()(Args&&... args);
```

Invokes the sink with the given arguments.

This function invokes the sink by type erasing the call arguments and passing them to SignalSink::invoke(). If neccesary (See SignalSink::invokeWithoutCallee()) it also passes a reference to the callee object first.

#### Parameter `cpp::SignalSink::operator()::args`<a id="cpp::SignalSink::operator()::args"></a>

``` cpp
Args&&... args
```

call arguments.

-----

### Function `cpp::SignalSink::callee`<a id="cpp::SignalSink::callee"></a>

``` cpp
SignalEmitter* callee() const;
```

Gives the callee of the connection.

-----

### Function `cpp::SignalSink::caller`<a id="cpp::SignalSink::caller"></a>

``` cpp
SignalEmitter* caller() const;
```

Gives the caller of the connection.

-----

-----
