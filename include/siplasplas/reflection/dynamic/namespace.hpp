#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_NAMESPACE_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_NAMESPACE_HPP

#include "entity.hpp"
#include "class.hpp"
#include <unordered_map>

namespace cpp
{

namespace dynamic_reflection
{

class Namespace : public Entity
{
public:
    using Metadata = SourceInfo;
    static std::shared_ptr<Namespace> create(const Metadata& metadata);
    static std::shared_ptr<Namespace> create(const std::string& fullName);
    static Namespace& fromEntity(const std::shared_ptr<Entity>& entity);

    Namespace& namespace_(const std::string& name);
    Class& class_(const std::string& name);

private:
    Namespace(const Metadata& metadata);
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_NAMESPACE_HPP
