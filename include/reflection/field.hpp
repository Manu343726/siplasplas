#ifndef SIPLASPLAS_REFLECTION_FIELD_HPP
#define SIPLASPLAS_REFLECTION_FIELD_HPP

#include "utility/preprocessor.hpp"
#include "reflection/detail/metaobject.hpp"

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
        
        template<typename T>
        class Binded
        {
        public:
            Binded(const Field& field, const T& object) :
                _field{&field},
                _object{const_cast<T*>(&object)}
            {}

            cpp::MetaObject get() const
            {
                return _field->get(*_object);
            }

            cpp::MetaObject get()
            {
                return _field->get(*_object);
            }

        private:
            T* _object;
            const Field* _field;
        };

        template<typename T>
        Binded<T> bind(const T& object)
        {
            return { *this, object };
        }

    private:
        cpp::MetaType _type;
        cpp::MetaType _declType;
        std::string _name;
        std::size_t _offset;
    };

    

#define SIPLASPLAS_REFLECTION_FIELD(Class, FieldName) ::cpp::Field{ SIPLASPLAS_PP_STR(FieldName), & Class :: FieldName,\
    offsetof(Class, FieldName) }
}

#endif // SIPLASPLAS_REFLECTION_FIELD_HPP
