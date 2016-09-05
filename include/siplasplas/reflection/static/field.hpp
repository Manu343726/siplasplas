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

template<typename SourceInfo_, typename Class, typename T, T Class::*field>
class Field<SourceInfo_, T Class::*, field>
{
public:
    using type = T Class::*;
    using value_type = T;
    using class_type = Class;
    using decay_t = cpp::meta::decay_t<value_type>;
    using SourceInfo = SourceInfo_;

    constexpr Field() = default;

    static constexpr type get()
    {
        return _field;
    }

    static constexpr const decay_t& get(const Class& object)
    {
        return object.*_field;
    }

    static constexpr decay_t& get(Class& object)
    {
        return object.*_field;
    }

    constexpr const decay_t& operator()(const Class& object) const
    {
        return get(object);
    }

    constexpr decay_t& operator()(Class& object) const
    {
        return get(object);
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

template<typename FieldType, FieldType field>
class Field : public codegen::Field<FieldType, field>
{};

}

}
#endif // SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP
