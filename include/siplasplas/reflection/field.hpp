#ifndef SIPLASPLAS_REFLECTION_FIELD_HPP
#define SIPLASPLAS_REFLECTION_FIELD_HPP

#include "detail/metaobject.hpp"

#include <type_traits>
#include <string>
#include <cstddef>

namespace cpp
{
    class Field
    {
    public:
        Field() = default;

        template<typename Class, typename T>
        Field(const std::string& name, T Class::* member, std::size_t offset) :
            _type{cpp::MetaType::get<std::decay_t<T>>()},
            _declType{cpp::MetaType::get<T>()},
            _name{name},
            _offset{offset}
        {}

        bool is_reference() const
        {
            return _type.type()(cpp::TypeTrait::is_reference);
        }

        const std::string& name() const
        {
            return _name;
        }

        template<typename T>
        cpp::MetaObject get(const T& object) const
        {
            return {_type, reinterpret_cast<char*>(const_cast<void*>(&object)) + _offset};   
        }

        template<typename T>
        cpp::MetaObject get(T& object)
        {
            return {_type, reinterpret_cast<char*>(&object) + _offset, cpp::MetaObject::ConstructReference};
        }

        const cpp::MetaType& type() const
        {
            return _type;
        }

        std::size_t offset() const
        {
            return _offset;
        }

    private:
        cpp::MetaType _type;
        cpp::MetaType _declType;
        std::string _name;
        std::size_t _offset;
    };
}

namespace cpp
{
    template<typename T>
    class BindedField : public ::cpp::Field
    {
    public:
        BindedField(const Field& field, const T& object) :
            Field{field},
            _object{const_cast<T*>(&object)}
        {}

        cpp::MetaObject get() const
        {
            return Field::get(*_object);
        }

        cpp::MetaObject get()
        {
            return Field::get(*_object);
        }

        template<typename U>
        operator const U&() const
        {
            return get().template get<U>();
        }

        template<typename U>
        operator U&()
        {
            return get().template get<U>();
        }

    private:
        T* _object;
    };
}

#endif // SIPLASPLAS_REFLECTION_FIELD_HPP
