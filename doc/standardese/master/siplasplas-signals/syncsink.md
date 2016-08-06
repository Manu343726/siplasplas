---
layout: standardese-doc
---

# Header file `syncsink.hpp`

``` cpp
#define SIPLASPLAS_SIGNALS_SYNCSINK_HPP 

#include "sink.hpp"

#include <siplasplas/reflection/dynamic/function_pointer.hpp>

namespace cpp
{
    class SyncSink;
}
```

## Class `cpp::SyncSink`<a id="cpp::SyncSink"></a>

``` cpp
class SyncSink
: public cpp::SignalSink
{
public:
    template <typename Caller, typename Function>
    SyncSink(Caller& caller, Function function);
    
    template <typename Caller, typename Callee, typename Function>
    SyncSink(Caller& caller, Callee& callee, Function function);
    
    virtual bool pull() override;
    
protected:
    virtual void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) override;
    
    virtual bool invokeWithoutCallee() const override;
};
```

Implements a direct connection to the destination function

Synchronous sinks have no special policy for function invocation but just invoke the function directly when a signal arrives. See SignalEmitter::connect().

### Function template `cpp::SyncSink::SyncSink<Caller, Function>`<a id="cpp::SyncSink::SyncSink<Caller, Function>"></a>

``` cpp
template <typename Caller, typename Function>
SyncSink(Caller& caller, Function function);
```

Creates a synchronous sink given a caller and a destination function.

See SignalEmitter::connect\_async()

#### Parameter `cpp::SyncSink::SyncSink::caller`<a id="cpp::SyncSink::SyncSink::caller"></a>

``` cpp
Caller& caller
```

Caller object. \\param function Function to be invoked when the signal is handled.

-----

### Function template `cpp::SyncSink::SyncSink<Caller, Callee, Function>`<a id="cpp::SyncSink::SyncSink<Caller, Callee, Function>"></a>

``` cpp
template <typename Caller, typename Callee, typename Function>
SyncSink(Caller& caller, Callee& callee, Function function);
```

Creates a synchronous sink given a caller, a callee, and a destination function.

See SignalEmitter::connect\_async()

#### Parameter `cpp::SyncSink::SyncSink::caller`<a id="cpp::SyncSink::SyncSink::caller"></a>

``` cpp
Caller& caller
```

Caller object. \\param callee Callee object. \\param function Function to be invoked when the signal is handled.

-----

### Function `cpp::SyncSink::pull`<a id="cpp::SyncSink::pull"></a>

``` cpp
virtual bool pull() override;
```

Does nothing.

Synchronous sinks directly invoke the destination function when SignalSink::operator() and SyncSink::invoke() are invoked, so pull() has no work to do.

@return Always returns true.

-----

-----
