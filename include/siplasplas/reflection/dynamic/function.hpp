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

    template<typename C, typename R, typename... Args>
    class FunctionAccess<R(C::*)(Args...)> : public FunctionAccessInterface
    {
    public:
        using type = R(C::*)(Args...);

        FunctionAccess(type ptr) :
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

    template<typename C, typename R, typename... Args>
    class FunctionAccess<R(C::*)(Args...) const> : public FunctionAccessInterface
    {
    public:
        using type = R(C::*)(Args...) const;

        FunctionAccess(type ptr) :
            _ptr{ptr}
        {}

        Object invoke(const std::vector<Object>& args) const override
        {
            return vector_call(_ptr, args);
        }

        Object invoke(const std::vector<Object>& args) override
        {
            throw std::logic_error{"Cannot invoke const method with non-const caller"};
        }

    private:
        type _ptr;
    };

    template<typename R, typename... Args>
    class FunctionAccess<R(Args...)> : public FunctionAccessInterface
    {
    public:
        using type = R(*)(Args...);

        FunctionAccess(type ptr) :
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
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FUNCTION_HPP
