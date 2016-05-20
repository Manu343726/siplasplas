#include "runtimeloader.hpp"

using namespace cpp;
using namespace cpp::dynamic_reflection;

RuntimeLoader::RuntimeLoader(const DynamicLibrary& library) :
    _library{library},
    _runtime{library.path()}
{
    // Pass the runtime to the dynamic library to fill it
    getRuntimeLoader().get<void(*)(void*)>()(&_runtime);
}

DynamicLibrary::Symbol& RuntimeLoader::getRuntimeLoader()
{
    return _library.getSymbol("SIPLASPLAS_REFLECTION_LOAD_RUNTIME");
}
