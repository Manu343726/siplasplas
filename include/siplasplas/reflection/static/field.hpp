#ifndef SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP
#define SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP

#include <siplasplas/utility/meta.hpp>

namespace cpp
{

namespace static_reflection
{

namespace meta
{

template<typename SourceInfo, typename F, F field>
class Field;

#ifndef SIPLASPLAS_DOXYGEN_RUNNING
template<typename SourceInfo_, typename Class, typename T, T Class::*field>
class Field<SourceInfo_, T Class::*, field>
#else
/**
 * \ingroup static-reflection
 * \brief Stores static reflection information of a member object
 */
class Field
#endif // SIPLASPLAS_DOXYGEN_RUNNING
{
public:
    /**
     * \brief Pointer to member type
     */
    using type = T Class::*;
    /**
     * \brief Type of the member value
     */
    using value_type = T;
    /**
     * \brief Type of the class the member object belongs to
     */
    using class_type = Class;
    /**
     * \brief Type of the member value without cv nor reference qualifiers
     */
    using decay_t = cpp::meta::decay_t<value_type>;
    /**
     * \brief Source information of the member object. See cpp::static_reflection::meta::SourceInfo
     */
    using SourceInfo = SourceInfo_;

    constexpr Field() = default;

    /**
     * \brief Returns a pointer to the member object
     */
    static constexpr type get()
    {
        return _field;
    }

    /**
     * \brief Returns the value of the field in a given object
     *
     * Equivalent to cpp::invoke(get(), object)
     *
     * \param object Object to read the field value from
     * \returns a const lvalue reference to the field
     */
    static constexpr const decay_t& get(const Class& object)
    {
        return object.*_field;
    }

    /**
     * \brief Returns the value of the field in a given object
     *
     * Equivalent to cpp::invoke(get(), object)
     *
     * \param object Object to read the field value from
     * \returns A non-const lvalue reference to the field
     */
    static constexpr decay_t& get(Class& object)
    {
        return object.*_field;
    }

private:
    static constexpr type _field = field;
};

}

namespace codegen
{

template<typename FieldType, FieldType field>
class Field :
    public static_reflection::meta::Field<
        static_reflection::meta::EmptySourceInfo<Field<FieldType, field>>,
        FieldType,
        field
    >
{};

}

/**
 * \ingroup static-reflection
 * \brief Returns static reflection information of a given pointer to member object
 *
 * \tparam FieldType Type of the pointer to member object, i.e. `T Class::*`
 * \tparam field Pointer to member object
 *
 * See cpp::static_reflection::meta::Field for specific informatio about the
 * returned information
 */
template<typename FieldType, FieldType field>
class Field : public codegen::Field<FieldType, field>
{};

}

}
#endif // SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP
