#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIMELOADER_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIMELOADER_HPP

#include <siplasplas/reflection/dynamic/export.hpp>
#include <siplasplas/reflection/dynamic/runtime.hpp>
#include <siplasplas/utility/dynamiclibrary.hpp>
#include <siplasplas/variant/optional.hpp>

namespace cpp
{

namespace dynamic_reflection
{

/**
 * \ingroup dynamic-reflection
 * \brief Loads dynamic reflection information from an external library
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT RuntimeLoader
{
public:
    /**
     * \brief Default constructs a runtime loader
     *
     * No loaded library is assigned and the runtime is empty
     */
    RuntimeLoader() = default;

    /**
     * \brief Initializes and loads a dynamic reflection runtime from
     * a given external library
     *
     * See load()
     *
     * \param library External dynamic library to load the
     * reflection data from
     */
    RuntimeLoader(const cpp::DynamicLibrary& library);

    /**
     * \brief Loads reflection information from the given external library
     *
     *  This function invokes the type exportation function of the external
     *  library to load the library exported types and functions. The runtime is
     *  reset to reference this library, and all the library types and functions
     *  are registered into this runtime.
     *
     *  The behavior is undefined if the library has no type export function
     *  See SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT_TYPES() macro.
     *
     * \param library External library to load reflection information from
     */
    void load(const cpp::DynamicLibrary& library);

    /**
     * \brief Returns a reference to the loaded runtime
     */
    Runtime& runtime();

private:
    Optional<cpp::DynamicLibrary> _library;
    cpp::dynamic_reflection::Runtime _runtime;

    cpp::DynamicLibrary::Symbol& getRuntimeLoader();
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIMELOADER_HPP
