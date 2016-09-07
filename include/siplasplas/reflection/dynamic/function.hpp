#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP

#include "entity.hpp"
#include <siplasplas/typeerasure/function.hpp>

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Function : public Entity
{
public:
    template<typename... Args>
    auto operator()(Args&&... args) const
    {
        return _functionPointer(std::forward<Args>(args)...);
    }

    template<typename... Args>
    auto operator()(Args&&... args)
    {
        return _functionPointer(std::forward<Args>(args)...);
    }

    const cpp::typeerasure::Function32& getFunction() const;

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

    cpp::typeerasure::Function32 _functionPointer;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
