#ifndef SIPLASPLAS_REFLECTION_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_FUNCTION_HPP

#include "reflection/detail/metatype.hpp"
#include "reflection/detail/metaobject.hpp"
#include "reflection/detail/metaobject_manip.hpp"
#include "reflection/attributes/attribute.hpp"

#include <ctti/type_id.hpp>

#include <vector>
#include <type_traits>
#include <unordered_map>
#include <stdexcept>

#include <siplasplas/utility/tuple.hpp>

namespace cpp
{
    class Function
    {
    public:
        Function() = default;

        template<typename F>
        Function(const std::string& name, F function,
                 const std::shared_ptr<cpp::attributes::Attribute>& attribute = nullptr) :
            _invoker{ new Invoker<F>{function} },
            _attribute{attribute},
            _name{ name }
        {}

        template<typename Class>
        auto operator()(Class& object)
        {
            return [this, &object = object](auto... args)
            {
                std::vector<cpp::MetaObject> typeErasedArgs{ cpp::MetaObject{args}... };

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
                std::vector<cpp::MetaObject> typeErasedArgs{ cpp::MetaObject{ args }... };

                return _invoker->invoke(&object, typeErasedArgs);
            };
        }

        const std::string& name() const
        {
            return _name;
        }

        cpp::MetaType returnType() const
        {
            return _invoker->returnType();
        }

        const std::vector <cpp::MetaType> &parameterTypes() const
        {
            return _invoker->parameterTypes();
        }

        bool isConst() const
        {
            return _invoker->isConst();
        }

        const std::shared_ptr<cpp::attributes::Attribute>& attribute() const
        {
            return _attribute;
        }

        std::shared_ptr<cpp::attributes::Attribute>& attribute()
        {
            return _attribute;
        }

    private:
        class InvokerInterface
        {
        public:
            virtual cpp::MetaType returnType() const = 0;
            virtual const std::vector<cpp::MetaType>& parameterTypes() const = 0;
            virtual bool isConst() const = 0;

            virtual cpp::MetaObject invoke(void* object, const std::vector<cpp::MetaObject>& args, cpp::attributes::Attribute* attribute = nullptr) const = 0;
            virtual cpp::MetaObject invoke(const void* object, const std::vector<cpp::MetaObject>& args, cpp::attributes::Attribute* attribute = nullptr) const = 0;
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

            cpp::MetaObject invoke(void* object, const std::vector<cpp::MetaObject>& args, cpp::attributes::Attribute* attribute) const override
            {
                if(attribute)
                {
                    auto args_ = attribute->processArguments(args);
                    auto return_value = cpp::vector_call(_function, *static_cast<Class*>(object), args_);

                    return attribute->processReturnValue(return_value);
                }
                else
                {
                    return cpp::vector_call(_function, *static_cast<Class*>(object), args);
                }
            }

            cpp::MetaObject invoke(const void* object, const std::vector<cpp::MetaObject>& args, cpp::attributes::Attribute*) const override
            {
                throw std::runtime_error{ "Wrong invoker, this calls a non-const member function" };
            }

            bool isConst() const override
            {
                return false;
            }

            cpp::MetaType returnType() const override
            {
                return cpp::MetaType::get<R>();
            }

            const std::vector<cpp::MetaType>& parameterTypes() const override
            {
                return getParamTypes();
            }

        private:
            R (Class::*_function)(Args...);
            std::shared_ptr<cpp::attributes::Attribute> _attribute;

            static const std::vector<cpp::MetaType>& getParamTypes()
            {
                static std::vector<cpp::MetaType> paramTypes{ cpp::MetaType::get<Args>()... };

                return paramTypes;
            }
        };

        template<typename R, typename Class, typename... Args>
        class Invoker<R(Class::*)(Args...) const> : public InvokerInterface
        {
        public:
            typedef R(Class::*FunctionType)(Args...) const;

            Invoker(FunctionType function, const std::shared_ptr<cpp::attributes::Attribute>& attribute = nullptr) :
                _function{ function }
            {}

            cpp::MetaObject invoke(void* object, const std::vector<cpp::MetaObject>& args, cpp::attributes::Attribute*) const override
            {
                throw std::runtime_error{ "Wrong invoker, this calls a const member function" };
            }

            cpp::MetaObject invoke(const void* object, const std::vector<cpp::MetaObject>& args, cpp::attributes::Attribute* attribute) const override
            {
                if(_attribute)
                {
                    auto args_ = _attribute->processArguments(args);
                    auto return_value = cpp::vector_call(_function, *static_cast<const Class*>(object), args_);

                    return _attribute->processReturnValue(return_value);
                }
                else
                {
                    return cpp::vector_call(_function, *static_cast<const Class*>(object), args);
                }
            }

            bool isConst() const override
            {
                return true;
            }

            cpp::MetaType returnType() const override
            {
                return cpp::MetaType::get<R>();
            }

            const std::vector<cpp::MetaType>& parameterTypes() const override
            {
                return getParamTypes();
            }

        private:
            FunctionType _function;
            std::shared_ptr<cpp::attributes::Attribute> _attribute;

            static const std::vector<cpp::MetaType>& getParamTypes()
            {
                static std::vector<cpp::MetaType> paramTypes{ cpp::MetaType::get<Args>()... };

                return paramTypes;
            }
        };

        std::shared_ptr<InvokerInterface> _invoker;
        std::shared_ptr<cpp::attributes::Attribute> _attribute;
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
        cpp::MetaObject operator()(Args&&... args) const
        {
            return Function::operator()(*const_cast<const T*>(_object))(std::forward<Args>(args)...);
        }

        template<typename... Args>
        cpp::MetaObject operator()(Args&&... args)
        {
            return Function::operator()(*_object)(std::forward<Args>(args)...);
        }

    private:
        T* _object;
    };
}

#endif // SIPLASPLAS_REFLECTION_FUNCTION_HPP
