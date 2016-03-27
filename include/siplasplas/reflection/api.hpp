#ifndef SIPLASPLAS_INCLUDE_REFLECTION_API_HPP
#define SIPLASPLAS_INCLUDE_REFLECTION_API_HPP

#include "metaclass.hpp"
#include "parser/annotations.hpp"

#include <ctti/type_id.hpp>

namespace cpp
{
    template<typename Class>
    constexpr ::cpp::MetaClassData& reflection()
    {
        return ::cpp::Reflection<ctti::type_id<Class>().hash()>::reflection();
    }

    template<std::size_t N>
    constexpr ::cpp::MetaClassData& reflection(const char (&typeName)[N])
    {
        return ::cpp::Reflection<ctti::id_from_name(typeName).hash()>::reflection();
    }

    template<typename Class>
    ::cpp::BindedMetaClassData<Class> reflection(const Class& object)
    {
        return {&object, &reflection<Class>()};
    }
}

#endif // SIPLASPLAS_INCLUDE_REFLECTION_API_HPP
