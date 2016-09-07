#include "runtimeloader.hpp"
#include <siplasplas/reflection/dynamic/logger.hpp>
#include <siplasplas/reflection/dynamic/type.hpp>
#include <siplasplas/reflection/static/sourceinfo.hpp>
#include <siplasplas/reflection/dynamic/api.hpp>
#include <siplasplas/typeerasure/function.hpp>
#include <siplasplas/utility/assert.hpp>
#include <siplasplas/reflection/dynamic/exporttypes.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;
using namespace cpp::reflection::dynamic;

SourceInfo getSourceInfo(void* sourceInfo)
{
    return *reinterpret_cast<SourceInfo*>(sourceInfo);
}

RuntimeLoader::RuntimeLoader(const DynamicLibrary& library)
{
    load(library);
}

struct Context
{
    Runtime* runtime;
    const DynamicLibrary* library;

    static Context get(void* context)
    {
        return *reinterpret_cast<Context*>(context);
    }
};

void RuntimeLoader::load(const DynamicLibrary& library)
{
    _library = library;
    _runtime.reset(library.path());

    auto loadClass = [](void* context, void* sourceInfo)
    {
        auto loaderContext = Context::get(context);
        log().warn("Class '{}' from library {} ignored, classes are not supported",
            getSourceInfo(sourceInfo).fullName(),
            loaderContext.library->path()
        );
    };
    auto loadEnum = [](void* context, void* sourceInfo)
    {
        auto loaderContext = Context::get(context);
        log().warn("Enum '{}' from library {} ignored, enums are not supported",
            getSourceInfo(sourceInfo).fullName(),
            loaderContext.library->path()
        );
    };
    auto loadEnumValue = [](void* context, void* sourceInfo, const char* name, std::int64_t value)
    {
        auto loaderContext = Context::get(context);
        log().warn("Enum value '{}::{}' ({}) from library {} ignored, enums are not supported",
            getSourceInfo(sourceInfo).fullName(),
            name,
            value,
            loaderContext.library->path()
        );
    };
    auto loadMethod = [](void* context, void* sourceInfo, void* functionRef)
    {
        auto loaderContext = Context::get(context);
        auto& function = *reinterpret_cast<cpp::typeerasure::Function<cpp::FixedSizeStorage<64>>*>(functionRef);

        SIPLASPLAS_ASSERT(function.kind() == cpp::FunctionKind::MEMBER_FUNCTION || function.kind() == cpp::FunctionKind::CONST_MEMBER_FUNCTION);

        log().warn("Method '{}' from library {} ignored, methods not supported",
            getSourceInfo(sourceInfo).fullName(),
            loaderContext.library->path()
        );
    };
    auto loadField = [](void* context, void* sourceInfo, void* functionRef)
    {
        auto loaderContext = Context::get(context);
        auto function = *reinterpret_cast<cpp::typeerasure::Function<cpp::FixedSizeStorage<64>>*>(functionRef);

        SIPLASPLAS_ASSERT(function.kind() == cpp::FunctionKind::MEMBER_OBJECT);

        log().warn("Field '{}' from library {} ignored, fields not supported",
            getSourceInfo(sourceInfo).fullName(),
            loaderContext.library->path()
        );
    };

    auto loader = getRuntimeLoader().get<TypeLoader>();
    Context context;
    context.runtime = &_runtime;
    context.library = &library;

    loader(
        &context,
        loadClass,
        loadEnum,
        loadEnumValue,
        loadMethod,
        loadField
    );
}

Runtime& RuntimeLoader::runtime()
{
    return _runtime;
}

DynamicLibrary::Symbol& RuntimeLoader::getRuntimeLoader()
{
    return _library->getSymbol("SIPLASPLAS_REFLECTION_DYNAMIC_LOADTYPES");
}
