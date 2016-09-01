#ifndef SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP
#define SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP

namespace cpp
{
namespace static_reflection
{
namespace meta
{

template<
    typename SourceInfo,
    typename EnumType,
    typename Constants,
    typename ConstantsNames
>
class Enum;

template<
    typename SourceInfo_,
    typename EnumType,
    EnumType... Constants,
    typename... ConstantsNames
>
class Enum<
    SourceInfo_,
    EnumType,
    ::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,
    ::cpp::meta::list<ConstantsNames...>
>
{
private:
    // An extra slot is needed just for the case no constants were
    // reflected (by default).
    static const char* _names[sizeof...(ConstantsNames)];

    static constexpr EnumType _values[sizeof...(Constants)] = {
        Constants...
    };

public:
    using type = EnumType;
    using SourceInfo = SourceInfo_;
    using names_array_t  = decltype(_names);
    using values_array_t = decltype(_values);

    constexpr Enum() = default;

    static constexpr const names_array_t& names()
    {
        return _names;
    }

    static constexpr const values_array_t& values()
    {
        return _values;
    }

    static const char* toString(const EnumType value)
    {
        for(std::size_t i = 0; i < sizeof...(Constants) + 1; ++i)
        {
            if(_values[i] == value)
            {
                return _names[i];
            }
        }

        throw std::logic_error{"unknown enum constant"};
    }

    static EnumType fromString(const char* name)
    {
        for(std::size_t i = 0; i < sizeof...(Constants) + 1; ++i)
        {
            if(streq(name, _names[i]))
            {
                return _values[i];
            }
        }

        throw std::logic_error{"Unknown enum constant"};
    }

private:
    static constexpr bool streq(const char* lhs, const char* rhs)
    {
        return lhs == rhs or ((*rhs and *lhs) ? (*lhs == *rhs) and streq(lhs + 1, rhs + 1) : (not *lhs and not *rhs));
    }
};

template<
    typename SourceInfo,
    typename EnumType,
    EnumType... Constants,
    typename... ConstantsNames
>
const char* Enum<
    SourceInfo,
    EnumType,
    ::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,
    ::cpp::meta::list<ConstantsNames...>
>::_names[] = {
    ::cpp::meta::StringToArray<ConstantsNames>::c_str()...
};

template<
    typename SourceInfo,
    typename EnumType,
    EnumType... Constants,
    typename... ConstantsNames
>
constexpr EnumType Enum<
    SourceInfo,
    EnumType,
    ::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,
    ::cpp::meta::list<ConstantsNames...>
>::_values[];


} // namespace meta

namespace codegen
{

template<typename EnumType>
class Enum : public static_reflection::meta::Enum<
    static_reflection::meta::EmptySourceInfo<Enum<EnumType>>,
    EnumType,
    ::cpp::meta::list<
        std::integral_constant<EnumType, static_cast<EnumType>(0)>
    >,
    ::cpp::meta::list<
        ::cpp::meta::string<>
    >
>
{};

}

template<typename EnumType>
class Enum : public codegen::Enum<EnumType>
{};

} // namespace static_reflection
} // namespace cpp
#endif // SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP
