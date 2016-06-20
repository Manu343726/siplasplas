#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_DETAIL_FUNCTIONPOINTER_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_DETAIL_FUNCTIONPOINTER_HPP

#include <siplasplas/utility/hash.hpp>
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
        assert(_ptr.get() != nullptr);
        return _ptr->invoke(
            pack_to_vector(std::forward<Args>(args)...)
        );
    }

    template<typename... Args>
    Object operator()(Args&&... args)
    {
        assert(_ptr.get() != nullptr);
        return _ptr->invoke(
            pack_to_vector(std::forward<Args>(args)...)
        );
    }

    Object invoke(const std::vector<Object>& args)
    {
        return _ptr->invoke(args);
    }

    std::size_t hashCode() const
    {
        return _ptr->hashCode();
    }

    friend bool operator==(const FunctionPointer& lhs, const FunctionPointer& rhs)
    {
        return lhs.hashCode() == rhs.hashCode();
    }

    friend bool operator!=(const FunctionPointer& lhs, const FunctionPointer& rhs)
    {
        return !(lhs == rhs);
    }

private:
    class FunctionAccessInterface
    {
    public:
        virtual Object invoke(const std::vector<Object>& args) const = 0;
        virtual Object invoke(const std::vector<Object>& args) = 0;
        virtual std::size_t hashCode() const = 0;
    };

    template<typename Type, bool ReturnsVoid = std::is_same<void, cpp::function_return_type<Type>>::value>
    class FunctionAccess : public FunctionAccessInterface
    {
    public:
        using type = Type;

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

        std::size_t hashCode() const override
        {
            return ::cpp::hash(_ptr);
        }

    protected:
        type _ptr;
    };

    template<typename Type>
    class FunctionAccess<Type, true> : public FunctionAccessInterface
    {
    public:
        using type = Type;

        FunctionAccess(type ptr) :
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

        std::size_t hashCode() const override
        {
            return ::cpp::hash(_ptr);
        }

    protected:
        type _ptr;
    };

    std::shared_ptr<FunctionAccessInterface> _ptr;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_DETAIL_FUNCTIONPOINTER_HPP
