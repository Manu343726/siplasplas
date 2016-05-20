#ifndef SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP
#define SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP

#include <siplasplas/utility/meta.hpp>
#include <ctti/type_id.hpp>
#include <array>
#include <string>

namespace cpp
{

namespace static_reflection
{

enum class Kind
{
    NAMESPACE,
    FUNCTION,
    ENUM,
    CLASS,
    FIELD,
    UNKNOWN
};

namespace meta
{

template<
    static_reflection::Kind Kind,
    typename FullName,
    typename Spelling,
    typename DisplayName,
    typename File,
    std::size_t Line
>
class SourceInfo;

template<
    static_reflection::Kind Kind,
    char... FullName,
    char... Spelling,
    char... DisplayName,
    char... File,
    std::size_t Line
>
class SourceInfo<
    Kind,
    ::cpp::meta::string<FullName...>,
    ::cpp::meta::string<Spelling...>,
    ::cpp::meta::string<DisplayName...>,
    ::cpp::meta::string<File...>,
    Line
>
{
public:
    static constexpr static_reflection::Kind kind()
    {
        return Kind;
    }

    static const char* fullName()
    {
        static constexpr const char _fullName[] = {FullName..., '\0'};
        return _fullName;
    }

    static const char* spelling()
    {
        static constexpr const char _spelling[] = {Spelling..., '\0'};
        return _spelling;
    }

    static const char* displayName()
    {
        static constexpr const char _displayName[] = {DisplayName..., '\0'};
        return _displayName;
    }

    static const char* file()
    {
        static constexpr const char _file[] = {File..., '\0'};
        return _file;
    }

    static constexpr std::size_t line()
    {
        return Line;
    }
};

namespace
{
    template<typename T>
    struct FullNameFromType
    {
        using type = T;
    };
}

template<
    static_reflection::Kind Kind,
    typename Type,
    char... Spelling,
    char... DisplayName,
    char... File,
    std::size_t Line
>
class SourceInfo<
    Kind,
    FullNameFromType<Type>,
    ::cpp::meta::string<Spelling...>,
    ::cpp::meta::string<DisplayName...>,
    ::cpp::meta::string<File...>,
    Line
>
{
public:
    static constexpr static_reflection::Kind kind()
    {
        return Kind;
    }

    static const char* fullName()
    {
        static const std::string _fullName = std::string("::") + std::string(ctti::type_id<Type>().name().c_str());
        return _fullName.c_str();
    }

    static const char* spelling()
    {
        static constexpr const char _spelling[] = {Spelling..., '\0'};
        return _spelling;
    }

    static const char* displayName()
    {
        static constexpr const char _displayName[] = {DisplayName..., '\0'};
        return _displayName;
    }

    static const char* file()
    {
        static constexpr const char _file[] = {File..., '\0'};
        return _file;
    }

    static constexpr std::size_t line()
    {
        return Line;
    }
};

template<typename T>
using EmptySourceInfo = SourceInfo<
    (std::is_enum<T>() ? Kind::ENUM : Kind::CLASS),
    FullNameFromType<T>,
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    0
>;

}

}

}

#endif // SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP
