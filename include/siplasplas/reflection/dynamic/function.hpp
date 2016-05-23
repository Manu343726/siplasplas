#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP

#include "object.hpp"
#include "object_manip.hpp"
#include "entity.hpp"

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
        return _functionAccess->invoke(pack_to_vector(std::forward<Args>(args)...));
    }

    template<typename... Args>
    Object operator()(Args&&... args)
    {
        return _functionAccess->invoke(pack_to_vector(std::forward<Args>(args)...));
    }

    template<typename FunctionPointer>
    static std::shared_ptr<Function> create(const SourceInfo& sourceInfo, FunctionPointer function)
    {
        return std::shared_ptr<Function>{ new Function{sourceInfo, function} };
    }

    static Function& fromEntity(const std::shared_ptr<Entity>& entity);

private:
    template<typename FunctionPointer>
    Function(const SourceInfo& sourceInfo, FunctionPointer function) :
        Entity{sourceInfo},
        _functionAccess{ new FunctionAccess<FunctionPointer>{function} }
    {}

    class FunctionAccessInterface
    {
    public:
        virtual ~FunctionAccessInterface() = default;

        virtual Object invoke(const std::vector<Object>& args) const = 0;
        virtual Object invoke(const std::vector<Object>& args) = 0;
    };

    std::unique_ptr<FunctionAccessInterface> _functionAccess;

    template<typename Function>
    class FunctionAccess;

    template<typename Type, bool ReturnsVoid = std::is_same<void, cpp::function_return_type<Type>>::value>
    class FunctionAccessCommon : public FunctionAccessInterface
    {
    public:
        using type = Type;

        FunctionAccessCommon(type ptr) :
            _ptr{ptr}
        {}

        Object invoke(const std::vector<Object>& args) const override
        {
            return vector_call(_ptr, args);
        }

        Object invoke(const std::vector<Object>& args) override
        {
            return vector_call(_ptr, args);
        }

    private:
        type _ptr;
    };

    template<typename Type>
    class FunctionAccessCommon<Type, true> : public FunctionAccessInterface
    {
    public:
        using type = Type;

        FunctionAccessCommon(type ptr) :
            _ptr{ptr}
        {}

        Object invoke(const std::vector<Object>& args) const override
        {
            vector_call(_ptr, args);
            return Object();
        }

        Object invoke(const std::vector<Object>& args) override
        {
            vector_call(_ptr, args);
            return Object();
        }

    private:
        type _ptr;
    };

    template<typename C, typename R, typename... Args>
    class FunctionAccess<R(C::*)(Args...)> : public FunctionAccessCommon<R(C::*)(Args...)>
    {
    public:
        using FunctionAccessCommon<R(C::*)(Args...)>::FunctionAccessCommon;
    };

    template<typename C, typename R, typename... Args>
    class FunctionAccess<R(C::*)(Args...) const> : public FunctionAccessCommon<R(C::*)(Args...) const>
    {
    public:
        using FunctionAccessCommon<R(C::*)(Args...) const>::FunctionAccessCommon;
    };

    template<typename R, typename... Args>
    class FunctionAccess<R(Args...)> : public FunctionAccessCommon<R(*)(Args...)>
    {
    public:
        using FunctionAccessCommon<R(*)(Args...)>::FunctionAccessCommon;
    };
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
