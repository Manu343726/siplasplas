#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_DETAIL_FUNCTIONPOINTER_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_DETAIL_FUNCTIONPOINTER_HPP

#include <siplasplas/utility/cloning_ptr.hpp>
#include "object_manip.hpp"

namespace cpp
{

namespace dynamic_reflection
{

class FunctionPointer
{
public:
    template<typename Pointer>
    FunctionPointer(Pointer pointer) :
        _ptr{ new FunctionAccess<Pointer>{pointer}}
    {}

    template<typename... Args>
    Object operator()(Args&&... args) const
    {
        return _ptr->invoke(
            pack_to_vector(std::forward<Args>(args)...)
        );
    }

    template<typename... Args>
    Object operator()(Args&&... args)
    {
        return _ptr->invoke(
            pack_to_vector(std::forward<Args>(args)...)
        );
    }

private:
    class FunctionAccessInterface : public Clonable
    {
    public:
        virtual Object invoke(const std::vector<Object>& args) const = 0;
        virtual Object invoke(const std::vector<Object>& args) = 0;
    };

    template<typename Function>
    class FunctionAccess;

    template<typename Type, bool ReturnsVoid = std::is_same<void, cpp::function_return_type<Type>>::value>
    class FunctionAccessCommon : public DefaultClone<FunctionAccessCommon<Type, ReturnsVoid>, FunctionAccessInterface>
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
    class FunctionAccessCommon<Type, true> : public DefaultClone<FunctionAccessCommon<Type, true>, FunctionAccessInterface>
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

    CloningPtr<FunctionAccessInterface> _ptr;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_DETAIL_FUNCTIONPOINTER_HPP
