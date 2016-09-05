#ifndef SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP
#define SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP

#include <siplasplas/utility/meta.hpp>
#include <string>
#include <limits>
#include <array>

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

#ifndef SIPLASPLAS_DOXYGEN_RUNNING // Doxygen shit
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
class Enum
#else

/**
 * \ingroup static-reflection
 * \brief Stores static reflection information of an enumeration type
 *
 * This class stores information of a given enumeration type and exposes it
 * as methods that can be used to query for enumeration values, get string representations
 * of enumeration values, indexing the enumeration, etc.
 */
class Enum
#endif // SIPLASPLAS_DOXYGEN_RUNNING
{
public:
    /**
     * \brief Alias to the enumeration type
     */
    using type = EnumType;
    /**
     * \brief Alias to the enumeration type
     */
    using Type = EnumType;
    /**
     * \brief Underlying integral type of the enumeration
     */
    using UnderlyingType = typename std::underlying_type<EnumType>::type;
    /**
     * \brief Source information of the enumeration type. See cpp::static_reflection::meta::SourceInfo
     */
    using SourceInfo = SourceInfo_;
    /**
     * \brief Array type returned by names()
     */
    using names_array_t  = std::array<const char*, sizeof...(ConstantsNames)>;
    /**
     * \brief Array type returned by values()
     */
    using values_array_t = const EnumType[sizeof...(Constants)];

    constexpr Enum() = default;

    /**
     * \brief Returns the number of constants declared in the enumeration type
     */
    static constexpr std::size_t count()
    {
        static_assert(sizeof...(Constants) == sizeof...(ConstantsNames), "");
        return sizeof...(Constants);
    }

    /**
     * \brief Returns the set of enum constants names
     * \returns A const reference to a constexpr std::array with C strings
     * containing tha names
     */
    static constexpr const names_array_t& names()
    {
        return _namesArray;
    }

    /**
     * \brief Returns the set of enum constants values
     * \returns A const reference to a constexpr C array with the EnumType
     * values
     */
    static constexpr const values_array_t& values()
    {
        return ::cpp::meta::PackToArray<EnumType, Constants...>::get();
    }

    /**
     * \brief Returns the value of the i-th constant of the enumeration
     *
     * Equivalent to values()[i]
     *
     * \param i Zero-indexed index of the constant. Constants are indexed from top
     * declared ones to the bottom:
     *     ``` cpp
     *     enum class JoystickAxis
     *     {
     *         X, // Enum<JoystickAxis>::value(0)
     *         Y  // Enum<JoystickAxis>::value(1)
     *     };
     *     ```
     * the behavior is undefined if `i >= count()`.
     */
    static constexpr EnumType value(std::size_t i)
    {
        return values()[i];
    }

    /**
     * \brief Returns the name of the i-th constant of the enumeration
     *
     * Equivalent to names()[i]
     *
     * \param i Zero-indexed index of the constant. Constants are indexed from top
     * declared ones to the bottom:
     *     ``` cpp
     *     enum class JoystickAxis
     *     {
     *         X, // Enum<JoystickAxis>::value(0)
     *         Y  // Enum<JoystickAxis>::value(1)
     *     };
     *     ```
     * the behavior is undefined if `i >= count()`.
     *
     * \returns A pointer to a constexpr null terminated C string with the name
     * of the i-th constant
     */
    static constexpr const char* name(std::size_t i)
    {
        return names()[i];
    }

    /**
     * \brief Returns a string representation of the given enumeration value
     *
     * If the value is the value of the i-th constant of the enumeration, returns
     * the i-th constant name. The behavior is undefined if the enumeration declaration
     * has no constant with that value (Such as when converting an arbitrary integral
     * value to the enumeration). See has().
     *
     * \param value Enumeration value
     *
     * \returns A pointer to a constexpr null terminated C string with the name
     * of the corresponding enumeration constant. This function doesn't qualify
     * its result with the enumeration type name, the name is returned as declared
     * in the enum declaration.
     */
    static constexpr const char* toString(const EnumType value)
    {
        return _toString(value, 0);
    }

    /**
     * \brief Returns an enumeration value from the given enumeration constant
     * name
     *
     * This function searches for an enumeration constant with the given name.
     * Is assumed that **the given name is not qualified with the enumeration type name**.
     * The behavior is undefined if there's no enumeration constant declared
     * with that name. See has().
     *
     * \param name Pointer to a null terminated C string with the non-qualified
     * constant name
     */
    static constexpr EnumType fromString(const char* name)
    {
        return _fromString(name, 0);
    }

    /**
     * \brief Checks if there's an enumeration constant with the given name
     *
     * This function searches for an enumeration constant with the given name.
     * Is assumed that **the given name is not qualified with the enumeration type name**.
     *
     * \param name Pointer to a null terminated C string with the name
     *
     * \returns True if there's a constant declared with that name, false otherwise.
     */
    static constexpr bool has(const char* name)
    {
        return _has(name, 0);
    }

    /**
     * \brief Checks if there's an enumeration constant with the given value
     * \returns True if there's a constant declared with the given value, false
     * otherwise
     */
    static constexpr bool has(const UnderlyingType value)
    {
        return _has(value, 0);
    }

private:
    static constexpr std::array<const char*, sizeof...(ConstantsNames)> _namesArray = {{::cpp::meta::StringToArray<ConstantsNames>::c_str()...}};
    static constexpr bool streq(const char* lhs, const char* rhs)
    {
        return ((*rhs and *lhs) ? (*lhs == *rhs) and streq(lhs + 1, rhs + 1) : (not *lhs and not *rhs));
    }

    static constexpr const char* _toString(const EnumType value, std::size_t i)
    {
        return (i < sizeof...(Constants)) ?
            ((value == Enum::value(i)) ? Enum::name(i) : _toString(value, i + 1))
        :
            nullptr;
    }

    static constexpr EnumType _fromString(const char* name, std::size_t i)
    {
        return (i < sizeof...(ConstantsNames)) ?
            (streq(name, Enum::name(i)) ? Enum::value(i) : _fromString(name, i + 1))
        :
            static_cast<EnumType>(std::numeric_limits<typename std::underlying_type<EnumType>::type>::max());
    }

    static constexpr bool _has(const char* name, std::size_t i)
    {
        return (i < sizeof...(ConstantsNames)) && (streq(name, Enum::name(i)) || _has(name, i + 1));
    }

    static constexpr bool _has(const UnderlyingType value, std::size_t i)
    {
        return (i < sizeof...(Constants)) && ((value == static_cast<UnderlyingType>(Enum::value(i))) || _has(value, i + 1));
    }
};

template<typename SourceInfo, typename EnumType, EnumType... Constants, typename... ConstantsNames>
constexpr typename Enum<
    SourceInfo,
    EnumType,
    ::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,
    ::cpp::meta::list<ConstantsNames...>
>::names_array_t Enum<
    SourceInfo,
    EnumType,
    ::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,
    ::cpp::meta::list<ConstantsNames...>
>::_namesArray;

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

/**
 * \ingroup static-reflection
 * \class Enum
 * \brief Returns static reflection information of the given
 * enumeration type.
 *
 * See cpp::static_reflection::meta::Enum template for the returned information.
 *
 * \tparam EnumType Must be an enumeration type. The behavior is undefined otherwise.
 *
 * \example examples/reflection/static/enum.cpp
 */
template<typename EnumType>
class Enum : public codegen::Enum<EnumType>
{};

} // namespace static_reflection
} // namespace cpp
#endif // SIPLASPLAS_REFLECTION_STATIC_ENUM_HPP
