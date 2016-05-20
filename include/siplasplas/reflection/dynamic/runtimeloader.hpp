#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIMELOADER_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIMELOADER_HPP

#include <siplasplas/reflection/dynamic/export.hpp>
#include <siplasplas/reflection/dynamic/runtime.hpp>
#include <siplasplas/utility/dynamiclibrary.hpp>

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT RuntimeLoader
{
public:
    RuntimeLoader(const cpp::DynamicLibrary& library);

    Runtime& runtime();

private:
    cpp::DynamicLibrary _library;
    cpp::dynamic_reflection::Runtime _runtime;

    cpp::DynamicLibrary::Symbol& getRuntimeLoader();
    cpp::DynamicLibrary::Symbol& getTypeLoader();
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIMELOADER_HPP
