#include "runtime.hpp"
#include "logger.hpp"

#include <siplasplas/utility/exception.hpp>
#include <stdexcept>

using namespace cpp;
using namespace cpp::dynamic_reflection;

Runtime::Runtime() :
    Runtime{"unnamed"}
{}

Runtime::Runtime(const std::string& name)
{
    reset(name);
}

void Runtime::clear()
{
    _entities.clear();

    // Add global namespace
    addEntity(
        Namespace::create(
            SourceInfo("", SourceInfo::Kind::NAMESPACE)
        )
    );
}

void Runtime::reset(const std::string& name)
{
    _name = name;
    clear();
}

const std::string& Runtime::name() const
{
    return _name;
}

Namespace& Runtime::namespace_()
{
    return namespace_("");
}

bool Runtime::hasEntity(const std::string& fullName) const
{
    return _entities.find(fullName) != _entities.end();
}

Namespace& Runtime::namespace_(const std::string& fullName)
{
    return Namespace::fromEntity(getEntity(fullName).pointer());
}

Entity& Runtime::getEntity(const std::string& fullName)
{
    auto it = _entities.find(fullName);

    if(it != _entities.end())
    {
        return *it->second;
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "No entity named '{}' registered in the runtime",
            fullName
        );
    }
}

void Runtime::addEntity(const std::shared_ptr<Entity>& entity)
{
    if(!hasEntity(entity->fullName()))
    {
        if(!entity->sourceInfo().scope().isGlobalScope())
        {
            auto parentScope = entity->sourceInfo().scope().parent();

            if(hasEntity(parentScope.fullName()))
            {
                auto& parent = getEntity(parentScope.fullName());
                // Try to add entity to its parent
                parent.addChild(entity);
                // Register the entity if succeeded
                _entities[entity->fullName()] = entity;
                reflection::dynamic::log().debug("[reflection runtime '{}'] Registered entity '{}'", name(), entity->fullName());
            }
            else
            {
                throw cpp::exception<std::runtime_error>(
                    "Parent entity ('{}') of entity '{}' is not registred",
                    parentScope.fullName(), entity->fullName()
                );
            }
        }
        else
        {
            _entities[entity->fullName()] = entity;
            entity->attach(*this);
        }

        SIPLASPLAS_ASSERT(!entity->detached() && entity->runtime() == *this)(
            "Entity '{}' detached or registered in other runtime. Entity runtime: {}. Runtime: {}",
            entity->fullName(),
            (entity->detached() ? "<detached>" : entity->runtime().name()),
            name()
        );
    }
    else
    {
        reflection::dynamic::log().warn(
            "The runtime already has an entity named '{}'",
            entity->fullName()
        );
    }
}

namespace cpp
{

namespace dynamic_reflection
{

bool operator==(const Runtime& lhs, const Runtime& rhs)
{
    return lhs.name() == rhs.name();
}

bool operator!=(const Runtime& lhs, const Runtime& rhs)
{
    return !(lhs == rhs);
}

}

}
