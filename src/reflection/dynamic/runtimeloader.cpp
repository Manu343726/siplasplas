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

SourceInfo getSourceInfo(const void* sourceInfo)
{
    return *reinterpret_cast<const SourceInfo*>(sourceInfo);
}

cpp::typeerasure::TypeInfo getTypeInfo(const void* typeInfo)
{
    return *reinterpret_cast<const cpp::typeerasure::TypeInfo*>(typeInfo);
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

    auto loadClass = [](void* context, const void* sourceInfoPtr, const void* typeInfoPtr)
    {
        auto loaderContext = Context::get(context);
        auto sourceInfo = getSourceInfo(sourceInfoPtr);
        auto typeInfo   = getTypeInfo(typeInfoPtr);

        log().debug("Loading class '{}' from library '{}'...",
            sourceInfo.fullName(),
            loaderContext.library->path()
        );

        loaderContext.runtime->addEntity(
            Class::create(
                sourceInfo,
                typeInfo
            )
        );

        log().info("Loaded class '{}' from library '{}'",
            sourceInfo.fullName(),
            loaderContext.library->path()
        );
    };
    auto loadEnum = [](
        void* context,
        const void* sourceInfoPtr,
        const void* typeInfoPtr,
        const void* underlyingTypeInfoPtr,
        std::size_t count,
        const char* names[],
        const std::int64_t values[])
    {
        auto loaderContext = Context::get(context);
        auto sourceInfo = getSourceInfo(sourceInfoPtr);
        auto typeInfo   = getTypeInfo(typeInfoPtr);
        auto underlyingTypeInfo = getTypeInfo(underlyingTypeInfoPtr);

        std::vector<std::string> namesCopy;
        std::vector<std::int64_t> valuesCopy;
        namesCopy.reserve(count);
        valuesCopy.reserve(count);

        log().debug("Loading enum '{}' from library '{}'...",
            sourceInfo.fullName(),
            loaderContext.library->path()
        );


        for(std::size_t i = 0; i < count; ++i)
        {
            namesCopy.emplace_back(names[i]);
            valuesCopy.emplace_back(values[i]);

            log().debug(" - Loading {}::{} ({})",
                sourceInfo.fullName(),
                names[i],
                values[i]
            );
        }

        loaderContext.runtime->addEntity(
            Enum::create(
                sourceInfo,
                typeInfo,
                underlyingTypeInfo,
                namesCopy,
                valuesCopy
            )
        );

        log().info("Loaded enum '{}' from library '{}'",
            sourceInfo.fullName(),
            loaderContext.library->path()
        );
    };
    auto loadEnumValue = [](void* context, const void* sourceInfo, const char* name, std::int64_t value)
    {
        auto loaderContext = Context::get(context);
        log().warn("Enum value '{}::{}' ({}) from library {} ignored, enums are not supported",
            getSourceInfo(sourceInfo).fullName(),
            name,
            value,
            loaderContext.library->path()
        );
    };
    auto loadMethod = [](void* context, const void* sourceInfoRef, void* functionRef)
    {
        auto loaderContext = Context::get(context);
        auto sourceInfo = getSourceInfo(sourceInfoRef);
        auto& function = *reinterpret_cast<cpp::dynamic_reflection::detail::MethodPointer*>(functionRef);

        SIPLASPLAS_ASSERT(function.kind() == cpp::FunctionKind::MEMBER_FUNCTION || function.kind() == cpp::FunctionKind::CONST_MEMBER_FUNCTION);

        log().info("loaded method '{}' from library '{}'",
            sourceInfo.fullName(),
            loaderContext.library->path()
        );

        loaderContext.runtime->addEntity(
            Function::create(
                sourceInfo,
                function
            )
        );
    };
    auto loadField = [](void* context, const void* sourceInfo, void* functionRef)
    {
        auto loaderContext = Context::get(context);
        auto function = *reinterpret_cast<cpp::dynamic_reflection::detail::FieldPointer*>(functionRef);

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
