#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP

#include "field.hpp"
#include "function.hpp"
#include "entity.hpp"
#include "type.hpp"

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Class : public Entity
{
public:
    static std::shared_ptr<Class> create(const SourceInfo& sourceInfo, const Type& type);
    static Class& fromEntity(const std::shared_ptr<Entity>& entity);

    Class& class_(const std::string& name);
    Field& field_(const std::string& name);
    Function& function_(const std::string& name);

    const Type& type() const;

private:
    Class(const SourceInfo& sourceInfo, const Type& type);

    Type _type;
};

} // dynamic_reflection
} // cpp

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
