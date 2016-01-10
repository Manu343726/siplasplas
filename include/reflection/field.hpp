#ifndef SIPLASPLAS_REFLECTION_FIELD_HPP
#define SIPLASPLAS_REFLECTION_FIELD_HPP

#include "reflection/detail/any.hpp"

namespace cpp
{
    class Field
    {
        template<typename Class, typename T>
        Field(const std::string& name, Class::*T member,  )
    private:
        cpp::TypeInfo _type;
        std::size_t _offset;
    };
}

#endif // SIPLASPLAS_REFLECTION_FIELD_HPP