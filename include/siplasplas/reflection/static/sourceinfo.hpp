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
class SourceInfo
{
public:
    static constexpr static_reflection::Kind kind()
    {
        return Kind;
    }

    static const char* fullName()
    {
        return ::cpp::meta::StringToArray<FullName>::c_str();
    }

    static const char* spelling()
    {
        return ::cpp::meta::StringToArray<Spelling>::c_str();
    }

    static const char* displayName()
    {
        return ::cpp::meta::StringToArray<DisplayName>::c_str();
    }

    static const char* file()
    {
        return ::cpp::meta::StringToArray<File>::c_str();
    }

    static constexpr std::size_t line()
    {
        return Line;
    }
};

template<typename T>
using EmptySourceInfo = SourceInfo<
    (std::is_enum<T>::value ? Kind::ENUM : Kind::CLASS),
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    0
>;

}

}

}

#endif // SIPLASPLAS_REFLECTION_STATIC_SOURCEINFO_HPP
