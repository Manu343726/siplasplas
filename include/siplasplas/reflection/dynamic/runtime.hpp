#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIME_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIME_HPP

#include "namespace.hpp"

namespace cpp
{

namespace dynamic_reflection
{

/**
 * \ingroup dynamic-reflection
 * \brief Provides access to dynamic type and function information at runtime
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Runtime
{
public:
    Runtime();

    /**
     * \brief Initializes a dynamic reflection runtime
     *
     * \param name Runtime identifier. This identifier will be used for
     * logging and debugging purposes. It can be used to differentiate
     * runtimes comming from different libraries (See cpp::dynamic_reflection::RuntimeLoaded)
     */
    Runtime(const std::string& name);

    /**
     * \brief Clears registered type information
     */
    void clear();

    /**
     * \brief Clears (resets) registered type information
     *
     * \param name New runtime identifier
     */
    void reset(const std::string& name);

    /**
     * \brief Returns reflection information about the global namespace
     *
     * \returns A reference to the global namespace entity
     */
    Namespace& namespace_();

    /**
     * \brief Returns reflection information about a given namespace
     *
     * \param fullName Full qualified namespace name
     *
     * \returns A reference to the namespace entity. Throws a `std::runtime_error`
     * exception if there's no namespace registered with that name
     */
    Namespace& namespace_(const std::string& fullName);

    /**
     * \brief Checks if an enity is registered with the given name
     *
     * \param fullName Entity full qualified name
     *
     * \returns True if the runtime has an entity registered with the given
     * full name. False instead
     */
    bool hasEntity(const std::string& fullName) const;

    /**
     * \brief Returns a reference to the specified entity
     *
     * \param fullName Entity full qualified name. The behavior is undefined if
     * no entity is registered with the given name. See hasEntity()
     *
     * \returns A reference to the entity
     */
    Entity& getEntity(const std::string& fullName);

    /**
     * \brief Returns a reference to the specified entity
     *
     * \tparam EntityType Entity type (cpp::dynamic_reflection::Class,
     * cpp::dynamic_reflection::Enum, etc). The behavior is undefined if
     * the type doesn't match the entity kind (See Entity::kind())
     *
     * \param fullName Entity full qualified name. The behavior is undefined if
     * no entity is registered with the given name. See hasEntity()
     *
     * \returns A reference to the entity
     */
    template<typename EntityType>
    EntityType& getEntity(const std::string& fullName)
    {
        return EntityType::fromEntity(getEntity(fullName).pointer());
    }

    /**
     * \brief Registers an entity in the dynamic reflection runtime
     *
     *  Registers the entity in the runtime and attaches it
     *  to it. If there's an entity already registered with the
     *  entity full name, no registration is performed.
     *
     * \param entity Pointer to an entity
     */
    void addEntity(const std::shared_ptr<Entity>& entity);

    /**
     * \brief Returns the runtime name
     */
    const std::string& name() const;

private:
    std::string _name;
    std::unordered_map<std::string, std::shared_ptr<Entity>> _entities;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIME_HPP
