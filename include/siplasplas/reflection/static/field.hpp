#ifndef SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP
#define SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP

namespace cpp
{

namespace static_reflection
{

namespace meta
{

template<typename AstInfo, typename F, F field, std::size_t offset = 0>
class Field;

template<typename AstInfo, typename Class, typename T, T Class::*field, std::size_t Offset>
class Field<AstInfo, T Class::*, field, Offset> : public AstInfo
{
public:
    using type = T Class::*;
    using value_type = T;
    using class_type = Class;
    using decay_t = std::decay_t<value_type>;

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

    static constexpr std::size_t offset()
    {
        return Offset;
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
        EmptyAstInfo<Field<FieldType, field>>,
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
