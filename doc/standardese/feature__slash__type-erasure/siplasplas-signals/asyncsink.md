---
layout: standardese-doc
---

# Header file `asyncsink.hpp`

``` cpp
#define SIPLASPLAS_SIGNALS_ASYNCSINK_HPP 

#include "sink.hpp"

#include <siplasplas/typeerasure/function.hpp>

#include <readerwriterqueue/readerwriterqueue.h>

#include <siplasplas/signals/export.hpp>

namespace cpp
{
    class AsyncSink;
}
```

## Class `cpp::AsyncSink`<a id="cpp::AsyncSink"></a>

``` cpp
class AsyncSink
: public cpp::SignalSink
{
public:
    template <typename Caller, typename Function>
    AsyncSink(Caller& caller, Function function);
    
    template <typename Caller, typename Callee, typename Function>
    AsyncSink(Caller& caller, Callee& callee, Function function);
    
    virtual bool pull() override;
    
    virtual ~AsyncSink();
    
protected:
    virtual void invoke() override;
    
    virtual bool invokeWithoutCallee() const override;
};
```

Implements an asynchronous signal sink suited for communication between different threads.

The AsyncSink class implements a sink that by default enqueues signal emissions in a thread safe queue. The destination function is invoked only when the user asks for incomming signals by pulling the sink. See AsyncSInk::pull().

### Function template `cpp::AsyncSink::AsyncSink<Caller, Function>`<a id="cpp::AsyncSink::AsyncSink<Caller, Function>"></a>

``` cpp
template <typename Caller, typename Function>
AsyncSink(Caller& caller, Function function);
```

Creates an asynchronous sink given a caller and a destination function.

See SignalEmitter::connect\_async()

#### Parameter `cpp::AsyncSink::AsyncSink::caller`<a id="cpp::AsyncSink::AsyncSink::caller"></a>

``` cpp
Caller& caller
```

Caller object. \\param function Function to be invoked when the signal is handled.

-----

### Function template `cpp::AsyncSink::AsyncSink<Caller, Callee, Function>`<a id="cpp::AsyncSink::AsyncSink<Caller, Callee, Function>"></a>

``` cpp
template <typename Caller, typename Callee, typename Function>
AsyncSink(Caller& caller, Callee& callee, Function function);
```

Creates an asynchronous sink given a caller, a callee, and a destination function.

See SignalEmitter::connect\_async()

#### Parameter `cpp::AsyncSink::AsyncSink::caller`<a id="cpp::AsyncSink::AsyncSink::caller"></a>

``` cpp
Caller& caller
```

Caller object. \\param callee Callee object. \\param function Function to be invoked when the signal is handled.

-----

### Function `cpp::AsyncSink::pull`<a id="cpp::AsyncSink::pull"></a>

``` cpp
virtual bool pull() override;
```

Checks whether there are queued signal emissions waiting to be handled and invokes the destination function for each of them.

The AsyncSink::pull() method fetches pending signals emissions (That is, argument sets from previous signal emissions from the caller object) and invokes the destination function using that arguments, once for each arguments set in the queue. This function continues invoking the destination function until the queue is empty. The destination function is executed from the same thread pull() was invoked.

@return true if at lest one pending signal emission was processed, false if the queue was empty.

-----

-----
