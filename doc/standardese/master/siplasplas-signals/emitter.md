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

#include <siplasplas/signals/export.hpp>

#include <memory>

#include <mutex>

namespace cpp
{
    template <typename Class>
    auto emit(const Class& object);
    
    template <typename Class>
    auto emit(Class& object);
}
```

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
