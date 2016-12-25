#include "dynamiclibrary.hpp"
#include "exception.hpp"

static std::pair<void*, std::string> loadLibrary(const std::string& libraryPath);
static std::pair<void*, std::string> loadSymbol(cpp::DynamicLibrary& library, const std::string& syumbolName);
static void closeLibrary(void* libraryHandle);


class DynamicLibraryDeleter
{
public:
    void operator()(void* handle)
    {
        closeLibrary(handle);
    }
};


#ifdef WIN32
#define NOMINMAX
#include <Windows.h>

using namespace cpp;

static std::pair<void*, std::string> loadLibrary(const std::string& libraryPath)
{
    void* handle = LoadLibrary(libraryPath.c_str());

    if (handle)
    {
        return{ handle, libraryPath };
    }
    else
    {
        throw cpp::exception<std::runtime_error>("Cannot load shared library {}", libraryPath);
    }
}

static std::pair<void*, std::string> loadSymbol(DynamicLibrary& library, const std::string& symbolName)
{
    void* handle = GetProcAddress(reinterpret_cast<HMODULE>(library.handle()), symbolName.c_str());

    if (handle)
    {
        return{ handle, symbolName };
    }
    else
    {
        throw cpp::exception<std::runtime_error>("Cannot load function '{}' from shared library {}", symbolName, library.path());
    }
}

static void closeLibrary(void* libraryHandle)
{
    FreeLibrary(reinterpret_cast<HMODULE>(libraryHandle));
}
#else
#include <dlfcn.h>
#if defined(__APPLE__)

using namespace cpp;

static std::pair<void*, std::string> loadLibrary(const std::string& libraryPath)
{
    const char* path = libraryPath.empty() ? nullptr : libraryPath.c_str();
    void* handle = dlopen(path, RTLD_LAZY);

    if (handle)
    {
        Dl_info info;

        // Load binary path information from dladdr() API if available, else
        // use the user provided path
        if (dladdr(handle, &info) || !info.dli_fname)
        {
            return{ handle, libraryPath };
        }
        else
        {
            return{ handle, info.dli_fname };
        }
    }
    else
    {
        throw std::runtime_error{ "Cannot load shared library " + std::string{ libraryPath } };
    }
}
#else // __APPLE__
#include <link.h>

using namespace cpp;

static std::pair<void*, std::string> loadLibrary(const std::string& libraryPath)
{
    const char* path = libraryPath.empty() ? nullptr : libraryPath.c_str();
    void* handle = dlopen(path, RTLD_LAZY);

    if (handle)
    {
        link_map* info;

        // Load binary path information from dlinfo() API if available, else
        // use the user provided path
        if (dlinfo(handle, RTLD_DI_LINKMAP, &info) || !info->l_name)
        {
            return{ handle, libraryPath };
        }
        else
        {
            return{ handle, info->l_name };
        }
    }
    else
    {
        throw std::runtime_error{ "Cannot load shared library " + std::string{ libraryPath } };
    }
}

#endif // __APPLE__

static std::pair<void*, std::string> loadSymbol(DynamicLibrary& library, const std::string& symbolName)
{
    void* handle = dlsym(library.handle(), symbolName.c_str());

    if (handle)
    {
        return {handle, symbolName };
    }
    else
    {
        throw std::runtime_error{ "Cannot load symbol '" + symbolName + "' from library " + library.path() };
    }
}

static void closeLibrary(void* libraryHandle)
{
    dlclose(libraryHandle);
}
#endif // WIN32

using namespace cpp;

DynamicLibrary::DynamicLibrary(void* libraryHandle, const std::string& libraryPath) :
    _libraryHandle{libraryHandle, DynamicLibraryDeleter()},
    _libraryPath{libraryPath}
{}

DynamicLibrary DynamicLibrary::load(const std::string& libraryPath)
{
    const auto& library = loadLibrary(libraryPath);

    return{ library.first, library.second };
}

void* DynamicLibrary::handle() const
{
    return _libraryHandle.get();
}

const std::string& DynamicLibrary::path() const
{
    return _libraryPath;
}

DynamicLibrary::Symbol::Symbol(DynamicLibrary& library, void* symbolHandle, const std::string& symbolName) :
    _symbolHandle{symbolHandle},
    _symbolName{symbolName},
    _library{&library}
{}

DynamicLibrary::Symbol DynamicLibrary::Symbol::load(DynamicLibrary& library, const std::string& symbolName)
{
    const auto& symbol = loadSymbol(library, symbolName);

    return{ library, symbol.first, symbol.second };
}

DynamicLibrary::Symbol& DynamicLibrary::getSymbol(const std::string& name)
{
    auto it = _loadedSymbols.find(name);

    if(it != _loadedSymbols.end())
    {
        return it->second;
    }
    else
    {
        return _loadedSymbols[name] = DynamicLibrary::Symbol::load(*this, name);
    }
}

void* DynamicLibrary::Symbol::handle()
{
    return _symbolHandle;
}

const void* DynamicLibrary::Symbol::handle() const
{
    return _symbolHandle;
}

const std::string& DynamicLibrary::Symbol::name() const
{
    return _symbolName;
}

