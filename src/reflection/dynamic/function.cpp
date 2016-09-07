#include "function.hpp"

#include <siplasplas/utility/exception.hpp>

using namespace cpp;
using namespace cpp::dynamic_reflection;

Function& Function::fromEntity(const std::shared_ptr<Entity>& entity)
{
    if(entity->sourceInfo().kind() == SourceInfo::Kind::FUNCTION)
    {
        return static_cast<Function&>(*entity);
    }
    else
    {
        throw cpp::exception<std::runtime_error>(
            "Entity '{}' is not a function",
            entity->fullName()
        );
    }
}

const cpp::typeerasure::Function32& Function::getFunction() const
{
    return _functionPointer;
}
