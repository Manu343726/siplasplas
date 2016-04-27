#ifndef SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP
#define SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP

namespace cpp
{

namespace static_reflection
{

template<typename AstInfo, typename F, F field>
class Field;

template<typename AstInfo, typename Class, typename T, T Class::*field>
class Field<AstInfo, T Class::*, field> : public AstInfo
{
public:
    using type = T Class::*;
    using value_type = T;
    using class_type = Class;
    using decay_t = std::decay_t<value_type>;

    constexpr Field() = default;

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

}

#endif // SIPLASPLAS_REFLECTION_STATIC_FIELD_HPP
