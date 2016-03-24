#ifndef SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP
#define SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP

#include <vector>
#include "reflection/detail/metaobject.hpp"
#include "reflection/detail/metaobject_manip.hpp"

namespace cpp
{

namespace attributes
{

class Attribute
{
public:
    virtual std::vector<cpp::MetaObject> processArguments(const std::vector<cpp::MetaObject>& args) = 0;
    virtual cpp::MetaObject processReturnValue(const cpp::MetaObject& returnValue) = 0;

    template<typename... Args>
    std::vector<cpp::MetaObject> processArguments(Args&&... args)
    {
        return processReturnValue(std::vector<cpp::MetaObject>{cpp::MetaObject{std::forward<Args>(args)}...});
    }
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
            returnValue.get<meta::get_t<0, typename function_signature<decltype(&AttrClass::processReturnValue)>::args>>()
        )};
    }
};

}

}

#endif // SIPLASPLAS_REFLECTION_ATTRIBUTES_ATTRIBUTE_HPP
