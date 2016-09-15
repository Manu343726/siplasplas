#include "entity.hpp"
#include "runtime.hpp"
#include <siplasplas/utility/exception.hpp>

using namespace cpp;
using namespace dynamic_reflection;

Entity::Entity(const SourceInfo& sourceInfo) :
    _parent{},
    _runtime{nullptr},
    _sourceInfo{sourceInfo}
{}

Entity& Entity::parent()
{
    return *_parent.lock();
}

Runtime& Entity::runtime()
{
    return *_runtime;
}

const SourceInfo& Entity::sourceInfo() const
{
    return _sourceInfo;
}

const std::string& Entity::name() const
{
    return _sourceInfo.name();
}

const std::string& Entity::fullName() const
{
    return _sourceInfo.fullName();
}

const SourceInfo::Kind& Entity::kind() const
{
    return _sourceInfo.kind();
}

bool Entity::detached() const
{
    return _runtime == nullptr;
}

bool Entity::orphan() const
{
    return _parent.expired();
}

std::shared_ptr<Entity> Entity::pointer()
{
    return shared_from_this();
}

void Entity::attach(Runtime& runtime)
{
    if(detached())
    {
        _runtime = &runtime;
    }
    else
    {
        throw cpp::exception<std::runtime_error>("Entity '{}' already attached", sourceInfo().fullName());
    }
}

void Entity::attach(const std::weak_ptr<Entity>& parent)
{
    auto sharedParent = parent.lock();

    if(orphan())
    {
        _parent = parent;

        if(detached())
        {
            attach(sharedParent->runtime());
        }
        else
        {
            if(sharedParent->runtime() != runtime())
            {
                throw cpp::exception<std::runtime_error>(
                    "Cannot attach entity '{0}' to parent entity '{1}' which lives in a different runtime "
                    "('{0}' lives in runtime '{2}', '{1}' lives in runtime '{3}')",
                    fullName(), sharedParent->fullName(),
                    runtime().name(), sharedParent->runtime().name()
                );
            }
        }
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Cannot attach entity '{0}' to parent entity '{1}', "
            "'{0}' already has a parent ('{2}')",
            fullName(), sharedParent->fullName(),
            this->parent().fullName()
        );
    }
}

Entity& Entity::getChildByFullName(const std::string& fullName)
{
    auto it = _children.find(fullName);

    if(it != _children.end())
    {
        return runtime().getEntity(fullName);
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' has no child '{}'",
            this->fullName(), fullName
        );
    }
}

Entity& Entity::getChildByName(const std::string& name)
{
    return getChildByFullName(Scope::fromParentScope(sourceInfo().scope(), name).fullName());
}

std::vector<std::string> Entity::getChildrenNamesByKind(const SourceInfo::Kind& kind)
{
    std::vector<std::string> childrenNames;

    for(const auto& fullName : _children)
    {
        const auto& entity = getChildByFullName(fullName);

        if(entity.kind() == kind)
        {
            childrenNames.push_back(fullName);
        }
    }

    return childrenNames;
}

void Entity::addChild(const std::shared_ptr<Entity>& entity)
{
    if(!isChild(entity))
    {
        entity->attach(pointer());
        _children.insert(entity->fullName());
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' already has a child entity '{}'",
            fullName(), entity->fullName()
        );
    }
}

bool Entity::isChildByFullName(const std::string& fullName) const
{
    return _children.find(fullName) != _children.end();
}

bool Entity::isChildByName(const std::string& name) const
{
    return isChildByFullName(Scope::fromParentScope(sourceInfo().scope(), name).fullName());
}

bool Entity::isChild(const std::shared_ptr<Entity>& entity) const
{
    return isChildByFullName(entity->fullName());
}

bool Entity::isChild(const Entity& entity) const
{
    return isChildByFullName(entity.fullName());
}

namespace cpp
{

namespace dynamic_reflection
{

bool operator==(const Entity& lhs, const Entity& rhs)
{
    return lhs.fullName() == rhs.fullName();
}

bool operator!=(const Entity& lhs, const Entity& rhs)
{
    return !(lhs == rhs);
}

}

}
