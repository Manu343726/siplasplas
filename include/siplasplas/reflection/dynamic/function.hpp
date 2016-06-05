#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP

#include "object.hpp"
#include "object_manip.hpp"
#include "entity.hpp"
#include "function_pointer.hpp"

namespace cpp
{

namespace dynamic_reflection
{

class Function : public Entity
{
public:
    template<typename... Args>
    Object operator()(Args&&... args) const
    {
        return _functionPointer(std::forward<Args>(args)...);
    }

    template<typename... Args>
    Object operator()(Args&&... args)
    {
        return _functionPointer(std::forward<Args>(args)...);
    }

    template<typename FunctionPointer>
    static std::shared_ptr<Function> create(const SourceInfo& sourceInfo, FunctionPointer function)
    {
        return std::shared_ptr<Function>{ new Function{sourceInfo, function} };
    }

    static Function& fromEntity(const std::shared_ptr<Entity>& entity);

private:
    template<typename FunctionPointer>
    Function(const SourceInfo& sourceInfo, FunctionPointer functionPointer) :
        Entity{sourceInfo},
        _functionPointer{functionPointer}
    {}

    FunctionPointer _functionPointer;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
