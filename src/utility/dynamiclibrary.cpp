#include "dynamiclibrary.hpp"
#include <link.h>
#include <dlfcn.h>

using namespace cpp;

DynamicLibrary::DynamicLibrary(void* libraryHandle, const std::string& libraryPath) :
    _libraryHandle{libraryHandle},
    _libraryPath{libraryPath}
{}

DynamicLibrary::~DynamicLibrary()
{
    dlclose(_libraryHandle);
}

DynamicLibrary DynamicLibrary::load(const std::string& libraryPath)
{
    const char* path = libraryPath.empty() ? nullptr : libraryPath.c_str();
    void* handle = dlopen(path, RTLD_LAZY);

    if(handle)
    {
        link_map* info;

        if(dlinfo(handle, RTLD_DI_LINKMAP, &info) || !info->l_name)
        {
            return {handle, libraryPath};
        }
        else
        {
            return {handle, info->l_name};
        }
    }
    else
    {
        throw std::runtime_error{"Cannot load shared library " + std::string{libraryPath}};
    }
}

void* DynamicLibrary::handle() const
{
    return _libraryHandle;
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
    void* handle = dlsym(library.handle(), symbolName.c_str());

    if(handle)
    {
        return {library, handle, symbolName};
    }
    else
    {
        throw std::runtime_error{"Cannot load symbol '" + symbolName + "' from library " + library.path()};
    }
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

