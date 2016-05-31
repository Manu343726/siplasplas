#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_ENTITY_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_ENTITY_HPP

#include "sourceinfo.hpp"
#include "detail/runtime_forward.hpp"
#include <siplasplas/reflection/dynamic/export.hpp>

#include <memory>
#include <unordered_set>

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Entity : public std::enable_shared_from_this<Entity>
{
public:
    virtual ~Entity() = default;

    Entity& parent();
    Runtime& runtime();

    const SourceInfo& sourceInfo() const;
    const std::string& name() const;
    const std::string& fullName() const;
    const SourceInfo::Kind& kind() const;

    bool detached() const;
    bool orphan() const;

    void attach(Runtime& runtime);
    void attach(const std::weak_ptr<Entity>& parent);

    void addChild(const std::shared_ptr<Entity>& entity);
    bool isChild(const std::shared_ptr<Entity>& entity) const;
    bool isChild(const Entity& entity) const;
    bool isChildByName(const std::string& name) const;
    bool isChildByFullName(const std::string& fullName) const;
    Entity& getChildByFullName(const std::string& fullName);
    Entity& getChildByName(const std::string& name);

    std::shared_ptr<Entity> pointer();

    friend bool operator==(const Entity& lhs, const Entity& rhs);
    friend bool operator!=(const Entity& lhs, const Entity& rhs);

protected:
    Entity(const SourceInfo& sourceInfo);

private:
    std::weak_ptr<Entity> _parent;
    Runtime* _runtime;
    SourceInfo _sourceInfo;
    std::unordered_set<std::string> _children;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_ENTITY_HPP
