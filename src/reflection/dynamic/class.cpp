#include "class.hpp"
#include <siplasplas/utility/exception.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;

Class::Class(const SourceInfo& sourceInfo, const Type& type) :
    Entity{sourceInfo},
    _type{type}
{}

std::shared_ptr<Class> Class::create(const SourceInfo& sourceInfo, const Type& type)
{
    return std::shared_ptr<Class>{ new Class{sourceInfo, type} };
}

Class& Class::fromEntity(const std::shared_ptr<Entity>& entity)
{
    if(entity->sourceInfo().kind() == SourceInfo::Kind::CLASS)
    {
        return static_cast<Class&>(*entity);
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' is not a class",
            entity->fullName()
        );
    }
}

Class& Class::class_(const std::string& name)
{
    return Class::fromEntity(getChildByName(name).pointer());
}

Field& Class::field_(const std::string& name)
{
    return Field::fromEntity(getChildByName(name).pointer());
}

Function& Class::function_(const std::string& name)
{
    return Function::fromEntity(getChildByName(name).pointer());
}

const Type& Class::type() const
{
    return _type;
}
