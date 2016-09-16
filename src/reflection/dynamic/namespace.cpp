#include "namespace.hpp"
#include "runtime.hpp"
#include <siplasplas/utility/string.hpp>

using namespace cpp;
using namespace dynamic_reflection;

Namespace::Namespace(const Namespace::Metadata& metadata) :
    Entity(metadata)
{
    assert(metadata.kind() == SourceInfo::Kind::NAMESPACE);
}

std::shared_ptr<Namespace> Namespace::create(const Namespace::Metadata& metadata)
{
    return std::shared_ptr<Namespace>{new Namespace(metadata)};
}

Namespace& Namespace::fromEntity(const std::shared_ptr<Entity>& entity)
{
    if(entity->sourceInfo().kind() == SourceInfo::Kind::NAMESPACE)
    {
        return static_cast<Namespace&>(*entity);
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' is not a namespace",
            entity->fullName()
        );
    }
}

Namespace& Namespace::namespace_(const std::string& name)
{
    return Namespace::fromEntity(getChildByName(name).pointer());
}

Class& Namespace::class_(const std::string& name)
{
    return Class::fromEntity(getChildByName(name).pointer());
}

Enum& Namespace::enum_(const std::string& name)
{
    return Enum::fromEntity(getChildByName(name).pointer());
}
