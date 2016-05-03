#ifndef SIPLASPLAS_REFLECTION_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_FUNCTION_HPP

#include "type.hpp"
#include "object.hpp"
#include "object_manip.hpp"
#include "attributes/attribute.hpp"

#include <ctti/type_id.hpp>

#include <vector>
#include <type_traits>
#include <unordered_map>
#include <stdexcept>

#include <siplasplas/utility/tuple.hpp>

namespace cpp
{

namespace dynamic_reflection
{

template<typename Function>
void registerFunctionTypes()
{
    using args = cpp::function_arguments<Function>;
    using return_type = cpp::function_return_type<Function>;

    Type::registerTypes<args>();
    Type::registerType<return_type>();
}

class Function
{
public:
    Function() = default;

    template<typename F>
    Function(const std::string& name, F function,
             const std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute>& attribute = nullptr) :
        _invoker{ new Invoker<F>{function} },
        _attribute{attribute},
        _name{ name }
    {
        registerFunctionTypes<F>();
    }

    template<typename Class>
    auto operator()(Class& object)
    {
        return [this, &object = object](auto... args)
        {
            std::vector<cpp::dynamic_reflection::Object> typeErasedArgs{ cpp::dynamic_reflection::Object{args}... };

            if (this->isConst())
            {
                return _invoker->invoke(const_cast<const void*>(reinterpret_cast<void*>(&object)), typeErasedArgs, _attribute.get());
            }
            else
            {
                return _invoker->invoke(&object, typeErasedArgs, _attribute.get());
            }
        };
    }

    template<typename Class>
    auto operator()(const Class& object) const
    {
        return [this, &object = object](auto... args)
        {
            std::vector<cpp::dynamic_reflection::Object> typeErasedArgs{ cpp::dynamic_reflection::Object{ args }... };

            return _invoker->invoke(&object, typeErasedArgs);
        };
    }

    const std::string& name() const
    {
        return _name;
    }

    cpp::dynamic_reflection::Type returnType() const
    {
        return _invoker->returnType();
    }

    const std::vector <cpp::dynamic_reflection::Type> &parameterTypes() const
    {
        return _invoker->parameterTypes();
    }

    bool isConst() const
    {
        return _invoker->isConst();
    }

    const std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute>& attribute() const
    {
        return _attribute;
    }

    std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute>& attribute()
    {
        return _attribute;
    }

private:
    class InvokerInterface
    {
    public:
        virtual cpp::dynamic_reflection::Type returnType() const = 0;
        virtual const std::vector<cpp::dynamic_reflection::Type>& parameterTypes() const = 0;
        virtual bool isConst() const = 0;

        virtual cpp::dynamic_reflection::Object invoke(void* object, const std::vector<cpp::dynamic_reflection::Object>& args, cpp::dynamic_reflection::attributes::Attribute* attribute = nullptr) const = 0;
        virtual cpp::dynamic_reflection::Object invoke(const void* object, const std::vector<cpp::dynamic_reflection::Object>& args, cpp::dynamic_reflection::attributes::Attribute* attribute = nullptr) const = 0;
    };

    template<typename Function>
    class Invoker
    {
        static_assert(sizeof(Function) != sizeof(Function), "Invalid member function type");

        Invoker(Function) {};
    };

    template<typename R, typename Class, typename... Args>
    class Invoker<R (Class::*)(Args...)> : public InvokerInterface
    {
    public:
        using FunctionType = R (Class::*)(Args...);

        Invoker(FunctionType function) :
            _function{ function }
        {}

        cpp::dynamic_reflection::Object invoke(void* object, const std::vector<cpp::dynamic_reflection::Object>& args, cpp::dynamic_reflection::attributes::Attribute* attribute) const override
        {
            if(attribute)
            {
                auto args_ = attribute->processArguments(args);
                auto return_value = cpp::dynamic_reflection::vector_call(_function, *static_cast<Class*>(object), args_);

                return attribute->processReturnValue(return_value);
            }
            else
            {
                return cpp::dynamic_reflection::vector_call(_function, *static_cast<Class*>(object), args);
            }
        }

        cpp::dynamic_reflection::Object invoke(const void* object, const std::vector<cpp::dynamic_reflection::Object>& args, cpp::dynamic_reflection::attributes::Attribute*) const override
        {
            throw std::runtime_error{ "Wrong invoker, this calls a non-const member function" };
        }

        bool isConst() const override
        {
            return false;
        }

        cpp::dynamic_reflection::Type returnType() const override
        {
            return cpp::dynamic_reflection::Type::get<R>();
        }

        const std::vector<cpp::dynamic_reflection::Type>& parameterTypes() const override
        {
            return getParamTypes();
        }

    private:
        R (Class::*_function)(Args...);
        std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute> _attribute;

        static const std::vector<cpp::dynamic_reflection::Type>& getParamTypes()
        {
            static std::vector<cpp::dynamic_reflection::Type> paramTypes{ cpp::dynamic_reflection::Type::get<Args>()... };

            return paramTypes;
        }
    };

    template<typename R, typename Class, typename... Args>
    class Invoker<R(Class::*)(Args...) const> : public InvokerInterface
    {
    public:
        typedef R(Class::*FunctionType)(Args...) const;

        Invoker(FunctionType function, const std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute>& attribute = nullptr) :
            _function{ function }
        {}

        cpp::dynamic_reflection::Object invoke(void* object, const std::vector<cpp::dynamic_reflection::Object>& args, cpp::dynamic_reflection::attributes::Attribute*) const override
        {
            throw std::runtime_error{ "Wrong invoker, this calls a const member function" };
        }

        cpp::dynamic_reflection::Object invoke(const void* object, const std::vector<cpp::dynamic_reflection::Object>& args, cpp::dynamic_reflection::attributes::Attribute* attribute) const override
        {
            if(_attribute)
            {
                auto args_ = _attribute->processArguments(args);
                auto return_value = cpp::dynamic_reflection::vector_call(_function, *static_cast<const Class*>(object), args_);

                return _attribute->processReturnValue(return_value);
            }
            else
            {
                return cpp::dynamic_reflection::vector_call(_function, *static_cast<const Class*>(object), args);
            }
        }

        bool isConst() const override
        {
            return true;
        }

        cpp::dynamic_reflection::Type returnType() const override
        {
            return cpp::dynamic_reflection::Type::get<R>();
        }

        const std::vector<cpp::dynamic_reflection::Type>& parameterTypes() const override
        {
            return getParamTypes();
        }

    private:
        FunctionType _function;
        std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute> _attribute;

        static const std::vector<cpp::dynamic_reflection::Type>& getParamTypes()
        {
            static std::vector<cpp::dynamic_reflection::Type> paramTypes{ cpp::dynamic_reflection::Type::get<Args>()... };

            return paramTypes;
        }
    };

    std::shared_ptr<InvokerInterface> _invoker;
    std::shared_ptr<cpp::dynamic_reflection::attributes::Attribute> _attribute;
    std::string _name;
};

template<typename T>
class BindedFunction : public Function
{
public:
    BindedFunction(const Function& function, const T& object) :
        Function{function},
        _object{const_cast<T*>(&object)}
    {}

    template<typename... Args>
    cpp::dynamic_reflection::Object operator()(Args&&... args) const
    {
        return Function::operator()(*const_cast<const T*>(_object))(std::forward<Args>(args)...);
    }

    template<typename... Args>
    cpp::dynamic_reflection::Object operator()(Args&&... args)
    {
        return Function::operator()(*_object)(std::forward<Args>(args)...);
    }

private:
    T* _object;
};

using Method = Function;

}
}

#endif // SIPLASPLAS_REFLECTION_FUNCTION_HPP
