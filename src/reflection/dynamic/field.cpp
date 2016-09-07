#include "field.hpp"
#include <siplasplas/utility/exception.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;

const cpp::typeerasure::Field32& Field::getField() const
{
    return _field;
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
