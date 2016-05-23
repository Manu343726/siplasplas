#include "field.hpp"
#include <siplasplas/utility/exception.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;

Object Field::get(const void* object) const
{
    return _fieldAccess->get(object);
}

Object Field::get(void* object)
{
    return _fieldAccess->get(object);
}

const Type& Field::type() const
{
    return _fieldAccess->type();
}

const Type& Field::declType() const
{
    return _fieldAccess->declType();
}

bool Field::isReference() const
{
    return _fieldAccess->isReference();
}

Field& Field::fromEntity(const std::shared_ptr<Entity>& entity)
{
    if(entity->sourceInfo().kind() == SourceInfo::Kind::FIELD)
    {
        return static_cast<Field&>(*entity);
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' is not a class field",
            entity->fullName()
        );
    }
}
