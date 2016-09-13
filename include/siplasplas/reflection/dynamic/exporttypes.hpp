#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_EXPORTTYPES_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_EXPORTTYPES_HPP

#include "sourceinfo.hpp"
#include <siplasplas/typeerasure/anystorage/fixedsize.hpp>
#include <siplasplas/typeerasure/function.hpp>
#include <siplasplas/reflection/static/api.hpp>
#include <siplasplas/utility/fusion.hpp>
#include <siplasplas/typeerasure/typeinfo.hpp>
#include "logger.hpp"

namespace cpp
{

namespace dynamic_reflection
{

namespace detail
{

template<typename Entity>
::cpp::dynamic_reflection::SourceInfo* sourceInfoRef()
{
    static auto sourceInfo = ::cpp::dynamic_reflection::SourceInfo::fromStaticSourceInfo<Entity>();
    return &sourceInfo;
}

template<typename T>
::cpp::typeerasure::TypeInfo* typeInfoRef()
{
    static auto typeInfo = ::cpp::typeerasure::TypeInfo::get<T>();
    return &typeInfo;
}

using MethodPointer = cpp::typeerasure::Function32;
using FieldPointer  = MethodPointer;

using EnumLoader      = void(*)(void* context, const void* sourceInfo, const void* typeInfo);
using EnumValueLoader = void(*)(void* context, const void* sourceInfo, const char*, std::int64_t);
using ClassLoader     = void(*)(void* context, const void* sourceInfo, const void* typeInfo);
using MethodLoader    = void(*)(void* context, const void* sourceInfo, void* method);
using FieldLoader     = void(*)(void* context, const void* sourceInfo, void* field);

template<typename T>
void loadEnum(
    void* context,
    EnumLoader loadEnum,
    EnumValueLoader loadEnumValue
)
{
    using Enum = ::cpp::static_reflection::Enum<T>;
    loadEnum(context, sourceInfoRef<Enum>(), typeInfoRef<T>());

    for(std::size_t i = 0; i < Enum::count(); ++i)
    {
        loadEnumValue(context, sourceInfoRef<Enum>(), Enum::name(i), static_cast<std::int64_t>(Enum::value(i)));
    }
}

template<typename T>
void loadClass(
    void* context,
    ClassLoader loadClass,
    EnumLoader loadEnum,
    EnumValueLoader loadEnumValue,
    MethodLoader loadMethod,
    FieldLoader loadField
)
{
    reflection::dynamic::log().debug("exporttypes.hpp: loadClass<{}>(context={})", ctti::type_id<T>().name(), context);

    using Class = ::cpp::static_reflection::Class<T>;
    static_assert(std::is_same<typename Class::type, T>::value, "???");

    auto* classSourceInfoRef = sourceInfoRef<Class>();

    reflection::dynamic::log().debug(" - fullname (from sourceinfo ref):      '{}'", classSourceInfoRef->fullName());
    reflection::dynamic::log().debug(" - fullname (From static source info):  '{}'", Class::SourceInfo::fullName());
    reflection::dynamic::log().debug(" - fullname (From dynamic source info): '{}'", ::cpp::dynamic_reflection::SourceInfo::fromStaticSourceInfo<Class>().fullName());

    loadClass(context, classSourceInfoRef, typeInfoRef<T>());

    ::cpp::foreach_type<typename Class::Methods>([=](auto type)
    {
        using Method = ::cpp::meta::type_t<decltype(type)>;
        auto methodPtr = MethodPointer(Method::get());

        loadMethod(context, sourceInfoRef<Method>(), &methodPtr);
    });

    ::cpp::foreach_type<typename Class::Fields>([=](auto type)
    {
        using Field = ::cpp::meta::type_t<decltype(type)>;
        auto fieldPtr = FieldPointer(Field::get());

        loadField(context, sourceInfoRef<Field>(), &fieldPtr);
    });

    ::cpp::foreach_type<typename Class::Classes>([=](auto type)
    {
        using ClassInfo = ::cpp::meta::type_t<decltype(type)>;
        using Class = ::cpp::meta::type_t<ClassInfo>;

        detail::loadClass<Class>(
            context,
            loadClass,
            loadEnum,
            loadEnumValue,
            loadMethod,
            loadField
        );
    });

    ::cpp::foreach_type<typename Class::Enums>([=](auto type)
    {
        using EnumInfo = ::cpp::meta::type_t<decltype(type)>;
        using Enum = ::cpp::meta::type_t<EnumInfo>;

        detail::loadEnum<Enum>(context, loadEnum, loadEnumValue);
    });
}

template<typename T>
std::enable_if_t<std::is_enum<T>::value> loadType(
    void* context,
    ClassLoader loadClass,
    EnumLoader loadEnum,
    EnumValueLoader loadEnumValue,
    MethodLoader loadMethod,
    FieldLoader loadField
)
{
    detail::loadEnum<T>(
        context,
        loadClass,
        loadEnum,
        loadEnumValue,
        loadMethod,
        loadField
    );
}

template<typename T>
std::enable_if_t<!std::is_enum<T>::value> loadType(
    void* context,
    ClassLoader loadClass,
    EnumLoader loadEnum,
    EnumValueLoader loadEnumValue,
    MethodLoader loadMethod,
    FieldLoader loadField
)
{
    detail::loadClass<T>(
        context,
        loadClass,
        loadEnum,
        loadEnumValue,
        loadMethod,
        loadField
    );
}

template<typename... Ts>
void loadTypes(
    void* context,
    ClassLoader loadClass,
    EnumLoader loadEnum,
    EnumValueLoader loadEnumValue,
    MethodLoader loadMethod,
    FieldLoader loadField
)
{
    ::cpp::foreach_type<Ts...>([=](auto type)
    {
        using Type = ::cpp::meta::type_t<decltype(type)>;

        detail::loadType<Type>(
            context,
            loadClass,
            loadEnum,
            loadEnumValue,
            loadMethod,
            loadField
        );
    });
}

}

#define SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT_TYPES(...)               \
extern "C" void SIPLASPLAS_REFLECTION_DYNAMIC_LOADTYPES(              \
    void* context,                                                    \
    ::cpp::dynamic_reflection::detail::ClassLoader loadClass,         \
    ::cpp::dynamic_reflection::detail::EnumLoader loadEnum,           \
    ::cpp::dynamic_reflection::detail::EnumValueLoader loadEnumValue, \
    ::cpp::dynamic_reflection::detail::MethodLoader loadMethod,       \
    ::cpp::dynamic_reflection::detail::FieldLoader loadField          \
)                                                                     \
{                                                                     \
    ::cpp::dynamic_reflection::detail::loadTypes<__VA_ARGS__>(        \
        context,                                                      \
        loadClass,                                                    \
        loadEnum,                                                     \
        loadEnumValue,                                                \
        loadMethod,                                                   \
        loadField                                                     \
    );                                                                \
}

using TypeLoader = void(*)(
    void*,
    ::cpp::dynamic_reflection::detail::ClassLoader,
    ::cpp::dynamic_reflection::detail::EnumLoader,
    ::cpp::dynamic_reflection::detail::EnumValueLoader,
    ::cpp::dynamic_reflection::detail::MethodLoader,
    ::cpp::dynamic_reflection::detail::FieldLoader
);

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_EXPORTTYPES_HPP
