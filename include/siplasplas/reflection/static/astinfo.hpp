#ifndef SIPLASPLAS_REFLECTION_STATIC_ASTINFO_HPP
#define SIPLASPLAS_REFLECTION_STATIC_ASTINFO_HPP

#include <siplasplas/utility/meta.hpp>
#include <array>

namespace cpp
{

namespace static_reflection
{

template<
    typename Spelling,
    typename DisplayName,
    typename File,
    std::size_t Line
>
class AstInfo;

template<
    char... Spelling,
    char... DisplayName,
    char... File,
    std::size_t Line
>
class AstInfo<
    meta::string<Spelling...>,
    meta::string<DisplayName...>,
    meta::string<File...>,
    Line
>
{
public:
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
using EmptyAstInfo = AstInfo<
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    ::cpp::meta::string<>,
    0
>;

}

}

#endif // SIPLASPLAS_REFLECTION_STATIC_ASTINFO_HPP
