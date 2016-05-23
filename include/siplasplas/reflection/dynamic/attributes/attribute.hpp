#ifndef SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP
#define SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP

#include <vector>
#include <siplasplas/reflection/dynamic/object.hpp>
#include <siplasplas/reflection/dynamic/object_manip.hpp>
#include <siplasplas/utility/type_variables.hpp>
#include <siplasplas/reflection/export.hpp>

namespace cpp
{

namespace dynamic_reflection
{

namespace attributes
{

class SIPLASPLAS_REFLECTION_EXPORT Attribute
{
public:
    virtual std::vector<cpp::dynamic_reflection::Object> processArguments(const std::vector<cpp::dynamic_reflection::Object>& args);
    virtual cpp::dynamic_reflection::Object processReturnValue(const cpp::dynamic_reflection::Object& returnValue);
};

template<typename AttrClass>
class ImplementAttribute : public Attribute
{
public:
    std::vector<cpp::dynamic_reflection::Object> processArguments(const std::vector<cpp::dynamic_reflection::Object>& args) override
    {
        return cpp::dynamic_reflection::tuple_to_vector(
            cpp::dynamic_reflection::vector_call(&AttrClass::processArguments, static_cast<AttrClass&>(*this), args)
        );
    }

    cpp::dynamic_reflection::Object processReturnValue(const cpp::dynamic_reflection::Object& returnValue) override
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
    std::vector<cpp::dynamic_reflection::Object> processArguments(const std::vector<cpp::dynamic_reflection::Object>& args) override
    {
        return cpp::dynamic_reflection::tuple_to_vector(
            cpp::dynamic_reflection::vector_call(static_cast<std::tuple<Args...>(AttributeFor::*)(Args...)>(processArguments), args)
        );
    }

    cpp::dynamic_reflection::Object processReturnValue(const cpp::dynamic_reflection::Object& returnValue) override
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

}

#endif // SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP
