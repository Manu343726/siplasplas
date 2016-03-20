#ifndef SIPLASPLAS_INCLUDE_REFLECTION_API_HPP
#define SIPLASPLAS_INCLUDE_REFLECTION_API_HPP

#include "metaclass.hpp"
#include "parser/annotations.hpp"

namespace cpp
{
    template<typename Class>
    ::cpp::MetaClassData& reflection()
    {
        return ::cpp::Reflection<Class>::reflection();
    }

    template<typename Class>
    ::cpp::BindedMetaClassData<Class> reflection(const Class& object)
    {
        return {&object, &reflection<Class>()};
    }
}

#endif // SIPLASPLAS_INCLUDE_REFLECTION_API_HPP
