#ifndef SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_STATIC_CLASS_HPP

#include <siplasplas/reflection/static/astinfo.hpp>
#include <siplasplas/utility/meta.hpp>

namespace cpp
{

namespace static_reflection
{

namespace meta
{

template<typename AstInfo, typename Class_, typename Methods_, typename Fields_>
class Class : public AstInfo
{
public:
    using class_type = Class_;
    using type = class_type;

    using Methods = Methods_;
    using Fields = Fields_;
};

}

namespace codegen
{
    template<typename T>
    class Class :
        public static_reflection::meta::Class<
            EmptyAstInfo<T>,
            T,
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
