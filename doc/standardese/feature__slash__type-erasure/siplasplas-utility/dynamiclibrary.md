---
layout: standardese-doc
---

# Header file `dynamiclibrary.hpp`

``` cpp
#define SIPLASPLAS_UTILITY_DYNAMICLIBRARY_HPP 

#include <string>

#include <unordered_map>

#include <siplasplas/utility/export.hpp>

#include <memory>

namespace cpp
{
    class DynamicLibrary;
}
```

## Class `cpp::DynamicLibrary`<a id="cpp::DynamicLibrary"></a>

``` cpp
class DynamicLibrary
{
public:
    static DynamicLibrary load(const std::string& libraryPath);
    
    class Symbol;
    
    Symbol& getSymbol(const std::string& name);
    
    void* handle() const;
    
    const std::string& path() const;
};
```

DynamicLibrary is a RAII abstraction over a OS-specific shared library handle. The library handle is refcounted and automatically released when there are no more DynamicLibrary instances referencing it. Symbols can be loaded from the library using getSymbol() function.

### Function `cpp::DynamicLibrary::load`<a id="cpp::DynamicLibrary::load"></a>

``` cpp
static DynamicLibrary load(const std::string& libraryPath);
```

Loads a shared library.

This function loads a shared library from the given path. It follows the path conventions of the underlying OS API, no special path handling is done from siplasplas side. On linux, after loading the library the dlinfo() API is invoked to get the right path to the library. DynamicLibrary class can only be instanced through this factory function.

``` cpp
auto myLib = DynamicLibrary::load("myLib.dll");
```

#### Parameter `cpp::DynamicLibrary::load::libraryPath`<a id="cpp::DynamicLibrary::load::libraryPath"></a>

``` cpp
const std::string& libraryPath
```

Path to the shared library. Can be relative to the executable working directory or absolute. The given path could not be the same registred into the DynamicLibrary instance (See details).

-----

### Class `cpp::DynamicLibrary::Symbol`<a id="cpp::DynamicLibrary::Symbol"></a>

``` cpp
class Symbol
{
public:
    Symbol() = default;
    
    const std::string& name() const;
    
    void* handle();
    
    const void* handle() const;
    
    template <typename T>
    T get();
    
    template <typename T>
    T& getObject();
    
    template <typename T>
    const T& getObject() const;
};
```

Represents a symbol loaded from a dynamic library.

This class provides access to a loaded symbol from an external library. The class wraps the raw pointer to the loaded symbol returned from the OS-specific API. You can access the loaded symbol address directly or use the convenient get() and getObject() functions that already apply type conversions to the symbol. Symbols are associated with a DynamicLibrary instanced. When its parent dynamic library goes out of scope symbols are invalidated and accessing them has undefined behavior.

``` cpp
// Load "external_function" symbol from our shared library:
auto externalFunction = DynamicLibrary::Symbol::load(myLib, "external_function");

// Read the symbol as a function pointer:
auto fptr = externalFunction.get<void(int)>();

// Invoke the external library function:
fptr(42);
```

#### Function `cpp::DynamicLibrary::Symbol::name`<a id="cpp::DynamicLibrary::Symbol::name"></a>

``` cpp
const std::string& name() const;
```

Returns the name of the symbol.

-----

#### Function `cpp::DynamicLibrary::Symbol::handle`<a id="cpp::DynamicLibrary::Symbol::handle"></a>

``` cpp
void* handle();
```

Returns the raw address of the loaded symbol.

-----

#### Function `cpp::DynamicLibrary::Symbol::handle`<a id="cpp::DynamicLibrary::Symbol::handle"></a>

``` cpp
const void* handle() const;
```

Returns the raw address of the loaded symbol.

-----

#### Function template `cpp::DynamicLibrary::Symbol::get<T>`<a id="cpp::DynamicLibrary::Symbol::get<T>"></a>

``` cpp
template <typename T>
T get();
```

Returns the symbol converted to the required type.

##### Template parameter `cpp::DynamicLibrary::Symbol::get<T>::T`<a id="cpp::DynamicLibrary::Symbol::get<T>::T"></a>

``` cpp
typename T
```

Type to convert the symbol to. Must be a pointer type.

-----

#### Function template `cpp::DynamicLibrary::Symbol::getObject<T>`<a id="cpp::DynamicLibrary::Symbol::getObject<T>"></a>

``` cpp
template <typename T>
T& getObject();
```

Returns the symbol interpreted as a reference to an object.

This function is useful when loading variables from the shared library. Loading objects with automatic or dynamic storage duration has undefined behavior.

``` cpp
// Load a global variable symbol from a DynamicLibrary:
auto myLibGlobalVariable = myLibrary.getSymbol("my_lib_global_variable");

// Interpret as an int and read its value:
std::cout << "myLib::my_lib_global_variable: " << myLibGlobalVariable.getObject<int>();
```

##### Template parameter `cpp::DynamicLibrary::Symbol::getObject<T>::T`<a id="cpp::DynamicLibrary::Symbol::getObject<T>::T"></a>

``` cpp
typename T
```

Value type to convert the symbol to.

-----

#### Function template `cpp::DynamicLibrary::Symbol::getObject<T>`<a id="cpp::DynamicLibrary::Symbol::getObject<T>"></a>

``` cpp
template <typename T>
const T& getObject() const;
```

Returns the symbol interpreted as a const reference to an object.

This function is useful when loading variables from the shared library. Loading objects with automatic or dynamic storage duration has undefined behavior.

``` cpp
// Load a global variable symbol from a DynamicLibrary:
auto myLibGlobalVariable = myLibrary.getSymbol("my_lib_global_variable");

// Interpret as an int and read its value:
std::cout << "myLib::my_lib_global_variable: " << myLibGlobalVariable.getObject<int>();
```

##### Template parameter `cpp::DynamicLibrary::Symbol::getObject<T>::T`<a id="cpp::DynamicLibrary::Symbol::getObject<T>::T"></a>

``` cpp
typename T
```

Value type to convert the symbol to.

-----

-----

### Function `cpp::DynamicLibrary::getSymbol`<a id="cpp::DynamicLibrary::getSymbol"></a>

``` cpp
Symbol& getSymbol(const std::string& name);
```

Loads a symbol from this shared library.

Library loaded symbols are cached by DynamicLibrary instances to avoid invoking the OS API again on an already loaded symbol. First call to getSymbol(\<symbol name\>) invokes DynamicLibrary::Symbol::loadSymbol() using \*this as first argument. Consecuent calls read symbols from the cache.

@return Reference to the cached symbol.

#### Parameter `cpp::DynamicLibrary::getSymbol::name`<a id="cpp::DynamicLibrary::getSymbol::name"></a>

``` cpp
const std::string& name
```

Name of the symbol to load.

-----

### Function `cpp::DynamicLibrary::handle`<a id="cpp::DynamicLibrary::handle"></a>

``` cpp
void* handle() const;
```

OS-specific shared library handle.

@return `void*` pointer representing the OS-specific library handle.

-----

### Function `cpp::DynamicLibrary::path`<a id="cpp::DynamicLibrary::path"></a>

``` cpp
const std::string& path() const;
```

Shared library path. See DynamicLibrary::load().

@return const reference to the string containing the path to the library.

-----

-----
