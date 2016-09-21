#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP

#include "field.hpp"
#include "function.hpp"
#include "entity.hpp"
#include "enum.hpp"
#include "type.hpp"
#include <siplasplas/typeerasure/typeinfo.hpp>

namespace cpp
{

namespace dynamic_reflection
{

/**
 * \ingroup dynamic-reflection
 * \brief Stores dynamic reflection information of a class type
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Class : public Entity
{
public:
    /**
     * \brief Creates a Class entity with the given type information
     *
     * \param sourceInfo Class source information
     * \param typeInfo Class type information
     *
     * \returns A shared pointer holding the class entity. The returned entity is detached
     * (See cpp::dynamic_reflection::Runtime::addEntity())
     */
    static std::shared_ptr<Class> create(const SourceInfo& sourceInfo, const cpp::typeerasure::TypeInfo& typeInfo);

    /**
     * \brief Returns a reference to a class entity from a pointer to an entity
     *
     * This function performs an entity cast from a generic entity pointer to a
     * class entity. The behavior is undefined if the entity is not a class
     * (See Entity::kind())
     *
     * \param entity A shared pointer to an entity
     *
     * \returns A reference to a class entity
     */
    static Class& fromEntity(const std::shared_ptr<Entity>& entity);

    /**
     * \brief Returns reflection information of a member enumeration type
     *
     * The behavior is undefined if the class has no public member enumeration
     * registered with that name
     *
     * \param name Name of the member enumeration type
     *
     * \returns A reference to the enumeration entity with the enum reflection information
     */
    Enum& enum_(const std::string& name);

    /**
     * \brief Returns reflection information of a member class type
     *
     * The behavior is undefined if the class has no public member class
     * registered with that name
     *
     * \param name Name of the member class type
     *
     * \returns A reference to the class entity with the class reflection information
     */
    Class& class_(const std::string& name);

    /**
     * \brief Returns reflection information of a member object
     *
     * The behavior is undefined if the class has no public member object
     * registered with that name
     *
     * \param name Name of the member object
     *
     * \returns A reference to the member object (field) entity with the member object reflection information
     */
    Field& field_(const std::string& name);

    /**
     * \brief Returns reflection information of a member function
     *
     * The behavior is undefined if the class has no public member function
     * registered with that name
     *
     * \param name Name of the member function
     *
     * \returns A reference to the function entity with the member function reflection information
     */
    Function& function_(const std::string& name);

    /**
     * \brief Returns dynamic type information of the class type
     */
    const cpp::typeerasure::TypeInfo& typeInfo() const;

    /**
     * \brief Default constructs an object of the class type
     *
     * \returns A cpp::Any32 object hosting the object. The any has methods and attributes
     * registered pointing to the member functions and objects of the class type, so the returned
     * object can be manipulated in an OOP way
     */
    cpp::Any32 create();

private:
    Class(const SourceInfo& sourceInfo, const cpp::typeerasure::TypeInfo& typeInfo);

    cpp::typeerasure::TypeInfo _typeInfo;
};

} // dynamic_reflection
} // cpp

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
