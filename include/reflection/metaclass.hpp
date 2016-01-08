#ifndef SIPLASPLAS_REFLECTION_METACLASS_HPP
#define SIPLASPLAS_REFLECTION_METACLASS_HPP

#include <ctti/type_id.hpp>
#include <utility>

namespace detail
{
    class Value
    {
        template<typename T>
        Value(T&& value) :
            _val{ new T{ std::forward<T>(value) } },
            _type{ ctti::type_id(value) }
        {}

        const ctti::type_id_t& typeId() const
        {
            return _type;
        }

    private:
        void* _val = nullptr;
        ctti::type_id_t  _type;
    };
    template<::ctti::detail::hash_t classNameHash>
    struct MetaClassRegistry 
    {
    };

    class MetaClassBase
    {
        
    };

    template<typename T>
    class MetaClass
    {
        static constexpr const char* className()
        {
            return ctti::type_id<T>.name().c_str();
        }
    };
}

#define METACLASS(Name) class Name : public ::cpp::MetaClass<Name>

#endif // SIPLASPLAS_REFLECTION_METACLASS_HPP
