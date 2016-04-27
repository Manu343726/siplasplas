#ifndef SIPLASPLAS_REFLECTION_DETAIL_FUNCTIONPOINTER_HPP
#define SIPLASPLAS_REFLECTION_DETAIL_FUNCTIONPOINTER_HPP

#include "metaobject.hpp"
#include "metaobject_manip.hpp"

namespace cpp
{

struct FunctionInfo
{
    enum class Kind
    {
        FREE_FUNCTION,
        METHOD,
        CONST_METHOD
    };

    Kind kind;
    MetaType returnType;
    std::vector<MetaType> argTypes;
};


class FunctionPointer
{
public:
    template<typename... Args>
    cpp::MetaObject operator()(Args&&... args) const
    {
        return _ptr->invoke(cpp::pack_to_vector(std::forward<Args>(args)...));
    }

    template<typename... Args>
    cpp::MetaObject operator()(Args&&... args)
    {
        return _ptr->invoke(cpp::pack_to_vector(std::forward<Args>(args)...));
    }

    const FunctionInfo& functionInfo() const
    {
        return _ptr->functionInfo();
    }

private:
    class FunctionInterface
    {
    public:
        FunctionInterface(FunctionInfo&& info) :
            _info{std::move(info)}
        {}
        virtual ~FunctionInterface() = default;

        virtual cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) const = 0;
        virtual cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) = 0;

        const FunctionInfo& functionInfo() const
        {
            return _info;
        }
    };

    std::shared_ptr<FunctionInterface> _ptr;

    template<typename Function>
    class Function;

    template<typename C, typename R, typename... Args>
    class Function<R(C::*)(Args...)> : public FunctionInterface
    {
    public:
        using type = R(C::*)(Args...);

        Function(type ptr) :
            FunctionInterface{FunctionInfo{
                FunctionInfo::Kind::METHOD,
                MetaType::get<R>,
                { MetaType::get<Args>()... }
            }},
            _ptr{ptr}
        {}

        cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) const override
        {
            return cpp::vector_call(_ptr, args);
        }

        cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) override
        {
           return cpp::vector_call(_ptr, args);
        }

    private:
        type _ptr;
    };

    template<typename C, typename R, typename... Args>
    class Function<R(C::*)(Args...> const> : public FunctionInterface
    {
    public:
        using type = R(C::*)(Args...) const;

        Function(type ptr) :
            FunctionInterface{FunctionInfo{
                FunctionInfo::Kind::CONST_METHOD,
                MetaType::get<R>,
                { MetaType::get<Args>()... }
            }},
            _ptr{ptr}
        {}

        cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) const override
        {
            return cpp::vector_call(_ptr, args);
        }

        cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) override
        {
            throw std::logic_error{"Cannot invoke const method with non-const caller"};
        }

    private:
        type _ptr;
    };

    template<typename R, typename... Args>
    class Function<R(Args...)>
    {
    public:
        using type = R(Args...);

        Function(type ptr) :
            FunctionInterface{FunctionInfo{
                FunctionInfo::Kind::CONST_METHOD,
                MetaType::get<R>,
                { MetaType::get<Args>()... }
            }},
            _ptr{ptr}
        {}

        cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) const override
        {
            return cpp::vector_call(_ptr, args);
        }

        cpp::MetaObject invoke(const std::vector<cpp::MetaObject>& args) override
        {
            return cpp::vector_call(_ptr, args);
        }

    private:
        type _ptr;
    };
};

}

#endif // SIPLASPLAS_REFLECTION_DETAIL_FUNCTIONPOINTER_HPP
