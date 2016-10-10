#ifndef SIPLASPLAS_TYPEERASURE_ANY_HPP
#define SIPLASPLAS_TYPEERASURE_ANY_HPP

#include "simpleany.hpp"
#include "function.hpp"
#include "field.hpp"
#include <siplasplas/utility/hash.hpp>

namespace cpp
{

/**
 * \ingroup type-erasure
 * \brief Implements a type-erased object container with
 * support for dynamic method and attribute invokation
 *
 * cpp::Any extends the interface of cpp::SimpleAny to
 * add dynamic manipulation of the object's interface.
 * cpp::Any keeps track of a set of methods and attributes the
 * instace has assigned, so it can behave as an instance of a class
 * with its interface and associated data members:
 *
 * ``` cpp
 * auto any = cpp::Any::create<MyClass>("hello");
 * any("method") = &MyCass::method;
 *
 * any("method")("hello, "s, "world"s); // Invokes <MyClass object>.method("hello, ", "world")
 *
 * any["field"] = &MyClass::field;
 * any["field"] = 42; // <MyClass object>.field = 42
 * ```
 *
 * The behavior is undefined if the user access a method or attribute the any object
 * doesn't have
 *
 * \tparam Storage Storage for the type erased object
 * \tparam FunctionsStorage Storage for the type erased methods. `Storage` by default
 * \tparam FunctionArgsStorage Storage for the type erased method call arguments. `FunctionsStorage` by default
 * \tparam AttributesStorage Storage for the type erased attributes. `Storage` by default
 */
template<
    typename Storage,
    typename FunctionsStorage = Storage,
    typename FunctionArgsStorage = FunctionsStorage,
    typename AttributesStorage = FunctionsStorage
>
class Any : public SimpleAny<Storage>
{
public:
    using Base = SimpleAny<Storage>;
    using SimpleAny<Storage>::SimpleAny;

    using Method = cpp::typeerasure::Function<FunctionsStorage, FunctionArgsStorage>;
    using Attribute = cpp::typeerasure::Field<AttributesStorage>;

    template<typename Class, typename... Args>
    static Any create(Args&&... args)
    {
        return SimpleAny<Storage>::template create<Class>(std::forward<Args>(args)...);
    }

    class MethodProxy
    {
    public:
        MethodProxy(Method& method, Any* this_) :
            _method{&method},
            _this{this_}
        {}

        template<typename... Args>
        auto operator()(Args&&... args)
        {
            return (*_method)(_this->getReference(), std::forward<Args>(args)...);
        }

        template<typename... Args>
        auto operator()(Args&&... args) const
        {
            return (*_method)(_this->getReference(), std::forward<Args>(args)...);
        }

        template<typename Callable>
        MethodProxy& operator=(Callable&& callable)
        {
            *_method = std::forward<Callable>(callable);
            return *this;
        }

        const Method& method() const
        {
            return *_method;
        }

        Method& method()
        {
            return *_method;
        }

    private:
        Method* _method;
        Any* _this;
    };

    class ConstMethodProxy
    {
    public:
        ConstMethodProxy(const Method& method, const Any* this_) :
            _method{&method},
            _this{this_}
        {}

        template<typename... Args>
        auto operator()(Args&&... args) const
        {
            return (*_method)(_this->getReference(), std::forward<Args>(args)...);
        }

        const Method& method() const
        {
            return *_method;
        }

    private:
        const Method* _method;
        const Any* _this;
    };

    class AttributeProxy
    {
    public:
        AttributeProxy(Attribute& attribute, Any* this_) :
            _attribute{&attribute},
            _this{this_}
        {}

        template<typename T>
        AttributeProxy& operator=(T&& value)
        {
            assign(
                std::forward<T>(value),
                cpp::meta::bool_<cpp::function_kind<std::decay_t<T>>() == cpp::FunctionKind::MEMBER_OBJECT>()
            );
            return *this;
        }

        template<typename T>
        T& get()
        {
            return _attribute->template getAs<T>(_this->simpleAny());
        }

        template<typename T>
        operator T&()
        {
            return get<T>();
        }

        template<typename T>
        const T& get() const
        {
            return _attribute->template getAs<T>(_this->simpleAny());
        }

        template<typename T>
        operator const T&() const
        {
            return get<T>();
        }

        const Attribute& attribute() const
        {
            return *_attribute;
        }

        Attribute& attribute()
        {
            return *_attribute;
        }

    private:
        Attribute* _attribute;
        Any* _this;

        template<typename Callable>
        void assign(Callable&& callable, cpp::meta::true_)
        {
            (*_attribute) = std::forward<Callable>(callable);
        }

        template<typename T>
        void assign(T&& value, cpp::meta::false_)
        {
            get<std::decay_t<T>>() = std::forward<T>(value);
        }
    };

    class ConstAttributeProxy
    {
    public:
        ConstAttributeProxy(const Attribute& attribute, const Any* this_) :
            _attribute{&attribute},
            _this{this_}
        {}

        template<typename T>
        const T& get() const
        {
            return _attribute->template getAs<T>(_this->simpleAny());
        }

        template<typename T>
        operator const T&() const
        {
            return get<T>();
        }

        const Attribute& attribute() const
        {
            return *_attribute;
        }

    private:
        const Attribute* _attribute;
        const Any* _this;
    };


    /**
     * \brief Gives access to an object method
     *
     * \param name Name of the object. If the object has no method with that
     * name, the behavior is undefined
     *
     * \returns A proxy to the method. This proxy can be assigned, to change
     * the method, or invoked.
     */
    MethodProxy operator()(const std::string& name)
    {
        return {_methods[name], this};
    }

    /**
     * \brief Gives access to an object method
     *
     * \param name Name of the object. If the object has no method with that
     * name, the behavior is undefined
     *
     * \returns A proxy to the method. This proxy can be assigned, to change
     * the method, or invoked.
     */
    ConstMethodProxy operator()(const std::string& name) const
    {
        SIPLASPLAS_ASSERT(hasMethod(name))("Class {} has no method named '{}'", Base::typeInfo().typeName(), name);
        return {_methods[name], this};
    }

    AttributeProxy operator[](const std::string& name)
    {
        return {_attributes[name], this};
    }

    ConstAttributeProxy operator[](const std::string& name) const
    {
        SIPLASPLAS_ASSERT(hasAttribute(name))("Class {} has no attribute named '{}'", Base::typeInfo().typeName(), name);
        return {_attributes[name], this};
    }

    /**
     * \brief Checks if the object has a method with a given name
     *
     * \param name name of the method
     *
     * \returns True if there's a method associated to that name, false otherwise
     */
    bool hasMethod(const std::string& name) const
    {
        auto it = _methods.find(name);

        if(it != _methods.end())
        {
            return !it->second.empty();
        }
        else
        {
            return false;
        }
    }

    /**
     * \brief Checks if the object has an attribute with a given name
     *
     * \param name name of the attribute
     *
     * \returns True if there's an attribute associated to that name, false otherwise
     */
    bool hasAttribute(const std::string& name) const
    {
        auto it = _attributes.find(name);

        if(it != _attributes.end())
        {
            return !it->second.empty();
        }
        else
        {
            return false;
        }
    }

    /**
     * \brief Returns the any object as a SimpleAny
     */
    ::cpp::SimpleAny<Storage>& simpleAny()
    {
        return *static_cast<::cpp::SimpleAny<Storage>*>(this);
    }

    /**
     * \brief Returns the any object as a SimpleAny
     */
    const ::cpp::SimpleAny<Storage>& simpleAny() const
    {
        return *static_cast<::cpp::SimpleAny<Storage>*>(this);
    }

private:
    cpp::HashTable<std::string, Method> _methods;
    cpp::HashTable<std::string, Attribute> _attributes;
};

/**
 * \ingroup type-erasure
 * \brief An Any with 8 byte deadpool storage
 */
using Any8  = Any<DeadPoolStorage<8>>;

/**
 * \ingroup type-erasure
 * \brief An Any with 16 byte deadpool storage
 */
using Any16 = Any<DeadPoolStorage<16>>;

/**
 * \ingroup type-erasure
 * \brief An Any with 32 byte deadpool storage
 */
using Any32 = Any<DeadPoolStorage<32>>;

/**
 * \ingroup type-erasure
 * \brief An Any with 64 byte deadpool storage
 */
using Any64 = Any<DeadPoolStorage<64>>;

/**
 * \ingroup type-erasure
 * \brief A non-owning Any that (const) references an existing object
 */
using ConstReferenceAny = Any<ConstNonOwningStorage, DeadPoolStorage<32>>;

/**
 * \ingroup type-erasure
 * \brief A non-owning Any that references an existing object
 */
using ReferenceAny = Any<NonOwningStorage, DeadPoolStorage<32>>;

}

#endif // SIPLASPLAS_TYPEERASURE_ANY_HPP
