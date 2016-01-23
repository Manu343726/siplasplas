#ifndef SIPLASPLAS_REFLECTION_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_FUNCTION_HPP

#include "reflection/detail/metatype.hpp"
#include "reflection/detail/metaobject.hpp"

#include <ctti/type_id.hpp>

#include <vector>
#include <type_traits>
#include <unordered_map>
#include <stdexcept>

namespace cpp
{
    class Function
    {
    public:
        Function() = default;

        template<typename F>
        Function(const std::string& name, F function) :
            _invoker{ new Invoker<F>{function} },
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
                    return _invoker->invoke(const_cast<const void*>(reinterpret_cast<void*>(&object)), typeErasedArgs);
                }
                else
                {
                    return _invoker->invoke(&object, typeErasedArgs);
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

        template<typename T>
        class Binded
        {
        public:
            Binded(const Function* function, const T* object) :
                _function{const_cast<Function*>(function)},
                _object{const_cast<T*>(object)}
            {}

            template<typename... Args>
            cpp::MetaObject operator()(Args&&... args) const
            {
                return (*const_cast<const Function*>(_function))(*const_cast<const T*>(_object))(std::forward<Args>(args)...);
            }

            template<typename... Args>
            cpp::MetaObject operator()(Args&&... args)
            {
                return (*_function)(*_object)(std::forward<Args>(args)...);
            }

        private:
            Function* _function;
            T* _object;
        };

        template<typename T>
        Binded<T> bind(const T& object)
        {
            return { this, &object };
        }


    private:
        class InvokerInterface
        {
        public:
            virtual cpp::MetaType returnType() const = 0;
            virtual const std::vector<cpp::MetaType>& parameterTypes() const = 0;
            virtual bool isConst() const = 0;

            virtual cpp::MetaObject invoke(void* object, const std::vector<cpp::MetaObject>& args) const = 0;
            virtual cpp::MetaObject invoke(const void* object, const std::vector<cpp::MetaObject>& args) const = 0;
        };

        template<typename Function>
        class Invoker
        {
            static_assert(sizeof(Function) != sizeof(Function), "Invalid member function type");

            Invoker(Function) {}
        };

        template<typename R, typename Class, typename... Args>
        class Invoker<R (Class::*)(Args...)> : public InvokerInterface
        {
        public:
            using FunctionType = R (Class::*)(Args...);

            Invoker(FunctionType function) :
                _function{ function }
            {}

            cpp::MetaObject invoke(void* object, const std::vector<cpp::MetaObject>& args) const override
            {
                return _invoke(object, args, std::index_sequence_for<Args...>{});
            }

            cpp::MetaObject invoke(const void* object, const std::vector<cpp::MetaObject>& args) const override
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

            template<std::size_t... Is>
            cpp::MetaObject _invoke(void* object, const std::vector<cpp::MetaObject>& args, std::index_sequence<Is...>) const
            {
                return ((*reinterpret_cast<Class*>(object)).*_function)(args[Is].get<Args>()...);
            }

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

            Invoker(FunctionType function) :
                _function{ function }
            {}

            cpp::MetaObject invoke(void* object, const std::vector<cpp::MetaObject>& args) const override
            {
                throw std::runtime_error{ "Wrong invoker, this calls a const member function" };
            }

            cpp::MetaObject invoke(const void* object, const std::vector<cpp::MetaObject>& args) const override
            {
                return _invoke(object, args, std::index_sequence_for<Args...>{});
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

            template<std::size_t... Is>
            cpp::MetaObject _invoke(const void* object, const std::vector<cpp::MetaObject>& args, std::index_sequence<Is...>) const
            {
                return (*(reinterpret_cast<const Class*>(object)).*_function)(args[Is].get<Args>()...);
            }

            static const std::vector<cpp::MetaType>& getParamTypes()
            {
                static std::vector<cpp::MetaType> paramTypes{ cpp::MetaType::get<Args>()... };

                return paramTypes;
            }
        };

        std::shared_ptr<InvokerInterface> _invoker;
        std::string _name;
    };

#define SIPLASPLAS_REFLECTION_FUNCTION(Class, FunctionName) ::cpp::Function{ SIPLASPLAS_PP_STR(FunctionName), & Class :: FunctionName }
}

#endif // SIPLASPLAS_REFLECTION_FUNCTION_HPP
