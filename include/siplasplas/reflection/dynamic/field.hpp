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

/**
 * \ingroup dynamic-reflection
 * \brief Stores dynamic reflection information of a member object
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Field : public Entity
{
public:
    /**
     * \brief Instances a field entity given its source information and a member object pointer
     *
     * \param sourceInfo Field source information
     * \param field Type erased member object pointer. Must be non-empty.
     *
     * \returns A shared pointer holding the field entity
     */
    static std::shared_ptr<Field> create(const SourceInfo& sourceInfo, const cpp::typeerasure::Field32& field);

    /**
     * \brief Returns a reference to a field entity from a pointer to an entity
     *
     * This function performs an entity cast from a generic entity pointer to a
     * field entity. The behavior is undefined if the entity is not a field
     * (See Entity::kind())
     *
     * \param entity A shared pointer to an entity
     *
     * \returns A reference to a field entity
     */
    static Field& fromEntity(const std::shared_ptr<Entity>& entity);

    /**
     * \brief Returns the pointer to the member object
     */
    const cpp::typeerasure::Field32& getField() const;

    /**
     * \brief Returns a reference to the member object of the given object
     *
     * Equivakent to getField().get(object)
     *
     * \param Object Object to get the member object from
     */
    template<typename Object>
    decltype(auto) get(Object&& object) const
    {
        return _field.get(std::forward<Object>(object));
    }

    /**
     * \brief Returns a reference to the member object of the given object
     *
     * Equivakent to getField().getAs<T>(object)
     *
     * \tparam T Value type of the member object. The behavior is undefined if T
     * is not the same type of the member object
     * \param Object Object to get the member object from
     */
    template<typename T, typename Object>
    decltype(auto) get(Object&& object) const
    {
        return _field.getAs<T>(std::forward<Object>(object));
    }

private:
    Field(const SourceInfo& sourceInfo, const cpp::typeerasure::Field32& field);

    cpp::typeerasure::Field32 _field;
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FIELD_HPP
