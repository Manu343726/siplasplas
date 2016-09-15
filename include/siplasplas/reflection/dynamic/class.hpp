#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP

#include "field.hpp"
#include "function.hpp"
#include "entity.hpp"
#include "type.hpp"
#include <siplasplas/typeerasure/typeinfo.hpp>

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Class : public Entity
{
public:
    static std::shared_ptr<Class> create(const SourceInfo& sourceInfo, const cpp::typeerasure::TypeInfo& typeInfo);
    static Class& fromEntity(const std::shared_ptr<Entity>& entity);

    Class& class_(const std::string& name);
    Field& field_(const std::string& name);
    Function& function_(const std::string& name);

    const cpp::typeerasure::TypeInfo& typeInfo() const;

    cpp::Any32 create();

private:
    Class(const SourceInfo& sourceInfo, const cpp::typeerasure::TypeInfo& typeInfo);

    cpp::typeerasure::TypeInfo _typeInfo;
};

} // dynamic_reflection
} // cpp

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
