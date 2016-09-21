#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_NAMESPACE_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_NAMESPACE_HPP

#include "entity.hpp"
#include "class.hpp"
#include "enum.hpp"
#include <unordered_map>

namespace cpp
{

namespace dynamic_reflection
{

/**
 * \ingroup dynamic-reflection
 * \brief Stores dynamic reflection information of a namespace
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Namespace : public Entity
{
public:
    /**
     * \brief Creates a namespace entity given its source information
     *
     * \param sourceInfo Nmaespace source information
     *
     * \returns A shared pointer holding the namespace entity
     */
    static std::shared_ptr<Namespace> create(const SourceInfo& sourceInfo);

    /**
     * \brief Returns a reference to a namespace entity from a pointer to an entity
     *
     * This namespace performs an entity cast from a generic entity pointer to a
     * namespace entity. The behavior is undefined if the entity is not a namespace
     * (See Entity::kind())
     *
     * \param entity A shared pointer to an entity
     *
     * \returns A reference to a field entity
     */
    static Namespace& fromEntity(const std::shared_ptr<Entity>& entity);

    /**
     * \brief Returns dynamic reflection information of a nested namespace
     *
     * \param name Nested namespace name
     *
     * \returns Reference to namespace entity with the information
     */
    Namespace& namespace_(const std::string& name);

    /**
     * \brief Returns reflection information of a member class type
     *
     * The behavior is undefined if the namespace has no nested class
     * registered with that name
     *
     * \param name Name of the member class
     *
     * \returns A reference to the class entity with the class reflection information
     */
    Class& class_(const std::string& name);

    /**
     * \brief Returns reflection information of a member enumeration type
     *
     * The behavior is undefined if the namespace has no member enumeration
     * registered with that name
     *
     * \param name Name of the member enumeration type
     *
     * \returns A reference to the enumeration entity with the enum reflection information
     */
    Enum&  enum_(const std::string& name);

private:
    Namespace(const SourceInfo& sourceInfo);
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_NAMESPACE_HPP
