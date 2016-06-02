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

    void* get() const
    {
        return _ptr->pointer();
    }

    friend bool operator==(const FunctionPointer& lhs, const FunctionPointer& rhs)
    {
        return lhs.get() == rhs.get();
    }

    friend bool operator!=(const FunctionPointer& lhs, const FunctionPointer& rhs)
    {
        return !(lhs == rhs);
    }

private:
    class FunctionAccessInterface : public Clonable
    {
    public:
        virtual Object invoke(const std::vector<Object>& args) const = 0;
        virtual Object invoke(const std::vector<Object>& args) = 0;
        virtual void* pointer() const = 0;
    };

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

    protected:
        type _ptr;

        void* address() const
        {
            return const_cast<void*>(reinterpret_cast<const void*>(&_ptr));
        }
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

    protected:
        type _ptr;

        void* address() const
        {
            return const_cast<void*>(reinterpret_cast<const void*>(&_ptr));
        }
    };

    template<typename Type, bool = std::is_pointer<Type>::value || std::is_member_function_pointer<Type>::value>
    class FunctionAccess : public FunctionAccessCommon<Type>
    {
    public:
        using FunctionAccessCommon<Type>::FunctionAccessCommon;

        void* pointer() const override
        {
            return reinterpret_cast<void*>(this->_ptr);
        }

        Clonable* clone() const override
        {
            return new FunctionAccess{*this};
        }
    };

    template<typename Type>
    class FunctionAccess<Type, false> : public FunctionAccessCommon<Type>
    {
    public:
        using FunctionAccessCommon<Type>::FunctionAccessCommon;

        void* pointer() const override
        {
            return FunctionAccessCommon<Type>::address();
        }

        Clonable* clone() const override
        {
            return new FunctionAccess{*this};
        }
    };

    std::shared_ptr<FunctionAccessInterface> _ptr;
};

}

}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_DETAIL_FUNCTIONPOINTER_HPP
