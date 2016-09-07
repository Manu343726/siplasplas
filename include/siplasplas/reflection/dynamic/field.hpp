#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_FIELD_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_FIELD_HPP

#include "entity.hpp"
#include <siplasplas/typeerasure/field.hpp>
#include <siplasplas/typeerasure/simpleany.hpp>
#include <siplasplas/typeerasure/any.hpp>

#include <type_traits>
#include <string>
#include <cstddef>

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Field : public Entity
{
public:
    template<typename Class, typename T>
    static std::shared_ptr<Field> create(const SourceInfo& sourceInfo, T Class::* field)
    {
        return std::shared_ptr<Field>{ new Field{sourceInfo, field} };
    }

    static Field& fromEntity(const std::shared_ptr<Entity>& entity);

    const cpp::typeerasure::Field32& getField() const;

    template<typename Object>
    decltype(auto) get(Object&& object) const
    {
        return _field.get(std::forward<Object>(object));
    }

    template<typename T, typename Object>
    decltype(auto) get(Object&& object) const
    {
        return _field.get<T>(std::forward<Object>(object));
    }

private:
    template<typename Class, typename T>
    Field(const SourceInfo& sourceInfo, T Class::* field) :
        Entity{sourceInfo},
        _field{field}
    {}

    cpp::typeerasure::Field32 _field;
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FIELD_HPP
