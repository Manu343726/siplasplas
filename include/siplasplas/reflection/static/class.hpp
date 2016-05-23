#ifndef SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP

#include "sourceinfo.hpp"
#include <siplasplas/utility/meta.hpp>

namespace cpp
{

namespace static_reflection
{

namespace meta
{

template<typename SourceInfo, typename Class_,
    typename Methods_,
    typename Fields_,
    typename Constructors_,
    typename Classes_,
    typename Enums_
>
class Class : public SourceInfo
{
public:
    using class_type = Class_;
    using type = class_type;

    using Methods = Methods_;
    using Fields = Fields_;
    using Constructors = Constructors_;
    using Classes = Classes_;
    using Enums = Enums_;
};

}

namespace codegen
{
    template<typename T>
    class Class :
        public static_reflection::meta::Class<
            static_reflection::meta::EmptySourceInfo<T>,
            T,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>,
            ::cpp::meta::list<>
        >
    {};
}

template<typename T>
class Class : public codegen::Class<T>
{
    using type = T;
};

}

}

#endif // SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP
