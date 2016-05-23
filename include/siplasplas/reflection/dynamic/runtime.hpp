#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIME_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIME_HPP

#include "namespace.hpp"

namespace cpp
{

namespace dynamic_reflection
{

class Runtime
{
public:
    Runtime(const std::string& name);

    Namespace& namespace_();
    Namespace& namespace_(const std::string& fullName);

    bool hasEntity(const std::string& fullName) const;
    Entity& getEntity(const std::string& fullName);

    void addEntity(const std::shared_ptr<Entity>& entity);

    const std::string& name() const;

private:
    std::string _name;
    std::unordered_map<std::string, std::shared_ptr<Entity>> _entities;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_RUNTIME_HPP
