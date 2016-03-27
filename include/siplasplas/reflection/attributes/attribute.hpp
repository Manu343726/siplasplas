#ifndef SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP
#define SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP

#include <vector>
#include "../detail/metaobject.hpp"
#include "../detail/metaobject_manip.hpp"
#include <siplasplas/utility/type_variables.hpp>

namespace cpp
{

namespace attributes
{

class Attribute
{
public:
    virtual std::vector<cpp::MetaObject> processArguments(const std::vector<cpp::MetaObject>& args);
    virtual cpp::MetaObject processReturnValue(const cpp::MetaObject& returnValue);
};

template<typename AttrClass>
class ImplementAttribute : public Attribute
{
public:
    std::vector<cpp::MetaObject> processArguments(const std::vector<cpp::MetaObject>& args) override
    {
        return cpp::tuple_to_vector(
            cpp::vector_call(&AttrClass::processArguments, static_cast<AttrClass&>(*this), args)
        );
    }

    cpp::MetaObject processReturnValue(const cpp::MetaObject& returnValue) override
    {
        return {static_cast<AttrClass*>(this)->processReturnValue(
            returnValue.get<cpp::function_argument<0, decltype(&AttrClass::processReturnValue)>>()
        )};
    }
};

template<typename Function>
class AttributeFor;

template<typename R, typename Class, typename... Args>
class AttributeFor<R(Class::*)(Args...)> : public Attribute
{
public:
    virtual std::tuple<Args...> processArguments(Args... args) = 0;
    virtual R processReturnValue(const R& returnValue) = 0;

private:
    std::vector<cpp::MetaObject> processArguments(const std::vector<cpp::MetaObject>& args) override
    {
        return cpp::tuple_to_vector(
            cpp::vector_call(static_cast<std::tuple<Args...>(AttributeFor::*)(Args...)>(processArguments), args)
        );
    }

    cpp::MetaObject processReturnValue(const cpp::MetaObject& returnValue) override
    {
        return processReturnValue(returnValue.get<R>());
    }
};

template<template<typename> class Attribute, typename... Args>
auto attribute_template_builder(Args&&... args)
{
    return [&](auto function_type)
    {
        return Attribute<typename decltype(function_type)::type>{std::forward<Args>(args)...};
    };
}

}

}

#endif // SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP
