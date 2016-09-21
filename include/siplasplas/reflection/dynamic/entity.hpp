#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_ENTITY_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_ENTITY_HPP

#include "sourceinfo.hpp"
#include "detail/runtime_forward.hpp"
#include <siplasplas/reflection/dynamic/export.hpp>

#include <memory>
#include <unordered_set>

namespace cpp
{

namespace dynamic_reflection
{

/**
 * \ingroup dynamic-reflection
 * \brief Represents a C++ semantic entity (Class, function, namespace, etc) and manages its
 * dynamic reflection information
 */
class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Entity : public std::enable_shared_from_this<Entity>
{
public:
    virtual ~Entity() = default;

    /**
     * \brief Returns a reference to the parent entity
     * The behavior is undefined if the entity has no parent entity (See orphan())
     */
    Entity& parent();

    /**
     * \brief Returns a reference to the runtime where the entity is registered
     *
     * The behavior is undefined if the entity is not attached to any runtime. See detached()
     */
    Runtime& runtime();

    /**
     * \brief Returns source information about the entity
     */
    const SourceInfo& sourceInfo() const;

    /**
     * \brief Returns the name of the entity
     */
    const std::string& name() const;

    /**
     * \brief Returns the full qualifies name of the entity
     */
    const std::string& fullName() const;

    /**
     * \brief Returns the kind (Namespace, class, enum, member function, etc) of the entity
     */
    const SourceInfo::Kind& kind() const;

    /**
     * \brief Checks whether the entity is not registered (attached) into a dynamic reflection runtime
     *
     * \returns True if the entity is not attached to a runtime. False otherwise
     */
    bool detached() const;

    /**
     * \brief Checks if the entity has no parent entity
     *
     * \returns True if the entity has no parent entity, true otherwisw
     */
    bool orphan() const;

    /**
     * \brief Attaches the entity to a given runtime
     *
     * The behavior is undefined if the entity is already attached to a dynamic reflection runtime
     *
     * \param runtime Runtime which the enity will be attached to
     */
    void attach(Runtime& runtime);

    /**
     * \brief Attaches the entity to the runtime of a given entity and links that enity
     * as parent
     *
     * The behavior is undefined if the entity is already attached to a dynamic reflection runtime or
     * if the entity already has a parent entity
     *
     * \param parent Pointer to an already-registered entity
     */
    void attach(const std::weak_ptr<Entity>& parent);

    /**
     * \brief Links the given entity as child of the entity
     *
     * This function links the given entity as a child entity of the entity object.
     * At the same time, the given entity links this entity as its parent.
     * The behavior is undefined if the given entity already has a parent (See orphan()).
     * If the entity object already has a child entity with the given entity full name,
     * the given entity is ignored
     *
     * \param entity Shared pointer to an orphan entity
     */
    void addChild(const std::shared_ptr<Entity>& entity);

    /**
     * \brief Checks if the given entity is a child entity of this entity
     *
     * \param entity Shared pointer to an entity
     *
     * \returns True if the given entity has this entity as parent, false otherwise
     */
    bool isChild(const std::shared_ptr<Entity>& entity) const;

    /**
     * \brief Checks if the given entity is a child entity of this entity
     *
     * \param entity Reference to an entity
     *
     * \returns True if the given entity has this entity as parent, false otherwise
     */
    bool isChild(const Entity& entity) const;

    /**
     * \brief Checks if the entity has a child entity with the given name
     *
     *  Equivalent to isChildByFullName(fullName() + <name>)
     *
     * \param name Name of the child entity
     *
     * \returns True there's a child entity with the given name. False otherwise
     */
    bool isChildByName(const std::string& name) const;

    /**
     * \brief Checks if the entity has a child entity with the given name
     *
     *  Equivalent to isChildByFullName(fullName() + <name>)
     *
     * \param name Name of the child entity
     *
     * \returns True there's a child entity with the given name. False otherwise
     */
    bool isChildByFullName(const std::string& fullName) const;

    /**
     * \brief Returns a child entity given its full name
     *
     * The behavior is undefined if the entity object has no linked child entity
     * with that full name
     *
     * \param fullName Child entity full name
     *
     * \returns A reference to the child entity
     */
    Entity& getChildByFullName(const std::string& fullName);

    /**
     * \brief Returns a child entity given its name
     *
     * The behavior is undefined if the entity object has no linked child entity
     * with that name. Equivalent to getChildByFullName(fullName() + <name>)
     *
     * \param fullName Child entity full name
     *
     * \returns A reference to the child entity
     */
    Entity& getChildByName(const std::string& name);

    /**
     * \brief Returns the names of the child entities with a given kind
     *
     * This function filters the set of child entities of the entity object,
     * returning only the names of those which kind is equal to the specified kind.
     * This can be useful to do quaries like "List the set of classes of a given namespace":
     *
     * ``` cpp
     * for(const std::string& entityName : entity.getChildrenNamesByKind(SourceInfo::Kind::ENUM))
     * {
     *     const auto& enum_ = Enum::fromEntity(entity.getChildByFullName(entityName).pointer());
     *
     *     std::cout << enum_.fullName() << std::endl;
     * }
     * ```
     *
     * \param kind Entity kind Requested entity kind
     *
     * \returns A vector with the full names of the children entities with the given kind
     */
    std::vector<std::string> getChildrenNamesByKind(const SourceInfo::Kind& kind);

    /**
     * \brief Returns the shared pointer holding the entity object
     */
    std::shared_ptr<Entity> pointer();

    /**
     * \brief Checks two entitties for equality
     * \returns True if the two entitites have the same full name. False otherwise
     */
    friend bool operator==(const Entity& lhs, const Entity& rhs);

    /**
     * \brief Checks two entitties for inequality
     *
     * Equivalent to !(lhs == rhs)
     *
     * \returns True if the two entitites have different full name. False otherwise
     */
    friend bool operator!=(const Entity& lhs, const Entity& rhs);

protected:
    Entity(const SourceInfo& sourceInfo);

private:
    std::weak_ptr<Entity> _parent;
    Runtime* _runtime;
    SourceInfo _sourceInfo;
    std::unordered_set<std::string> _children;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_ENTITY_HPP
