#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP

#include "field.hpp"
#include "function.hpp"
#include "entity.hpp"

namespace cpp
{

namespace dynamic_reflection
{

class Class : public Entity
{
public:
    static std::shared_ptr<Class> create(const SourceInfo& sourceInfo);
    static Class& fromEntity(const std::shared_ptr<Entity>& entity);

    Class& class_(const std::string& name);
    Field& field_(const std::string& name);
    Function& function_(const std::string& name);

private:
    using Entity::Entity;
};

} // dynamic_reflection
} // cpp

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
