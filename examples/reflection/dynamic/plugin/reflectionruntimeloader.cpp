#include "reflectionruntimeloader.hpp"

#include "plugin.hpp"

#include <siplasplas/utility/fusion.hpp>
#include <siplasplas/reflection/dynamic/api.hpp>

using namespace cpp::dynamic_reflection;

void SIPLASPLAS_REFLECTION_LOAD_RUNTIME(void* runtimePtr)
{
    auto& runtime = *reinterpret_cast<Runtime*>(runtimePtr);

    runtime.addEntity(
        Class::create(
            SourceInfo::fromStaticSourceInfo<
                cpp::static_reflection::Class<Plugin>
            >(),
            Type::get<Plugin>()
        )
    );

    cpp::foreach_type<cpp::static_reflection::Class<Plugin>::Methods>([&](auto method)
    {
        using Method = typename decltype(method)::type;

        runtime.addEntity(
            Function::create(
                SourceInfo::fromStaticSourceInfo<Method>(),
                Method::get()
            )
        );
    });
}
