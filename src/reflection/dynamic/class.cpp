#include "class.hpp"
#include <siplasplas/utility/exception.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;

std::shared_ptr<Class> Class::create(const SourceInfo& sourceInfo)
{
    return std::shared_ptr<Class>{ new Class{sourceInfo} };
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
