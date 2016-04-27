#ifndef SIPLASPLAS_INCLUDE_REFLECTION_API_HPP
#define SIPLASPLAS_INCLUDE_REFLECTION_API_HPP

#include "metaclass.hpp"
#include "annotations.hpp"
#include "static/api.hpp"
#include <siplasplas/reflection/export.hpp>
#include <ctti/type_id.hpp>

namespace cpp
{
    template<typename Class>
    constexpr ::cpp::MetaClassData& reflection()
    {
        return ::cpp::Reflection<Class>::reflection();
    }

    template<typename Class>
    ::cpp::BindedMetaClassData<Class> reflection(const Class& object)
    {
        return {&object, &reflection<Class>()};
    }

    SIPLASPLAS_REFLECTION_EXPORT ::cpp::MetaClassData& reflection(const cpp::MetaType& type);
}
#endif // SIPLASPLAS_INCLUDE_REFLECTION_API_HPP
