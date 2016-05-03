#ifndef SIPLASPLAS_REFLECTION_FIELD_HPP
#define SIPLASPLAS_REFLECTION_FIELD_HPP

#include "object.hpp"

#include <type_traits>
#include <string>
#include <cstddef>

namespace cpp
{

namespace dynamic_reflection
{

class Field
{
public:
    Field() = default;

    template<typename Class, typename T>
    Field(const std::string& name, T Class::* member, std::size_t offset) :
        _type{cpp::dynamic_reflection::Type::get<std::decay_t<T>>()},
        _declType{cpp::dynamic_reflection::Type::get<T>()},
        _name{name},
        _offset{offset}
    {
        Type::registerType<T>();
    }

    bool is_reference() const
    {
        return _type.type()(cpp::TypeTrait::is_reference);
    }

    const std::string& name() const
    {
        return _name;
    }

    template<typename T, typename std::enable_if_t<
        !std::is_same<void*, T>::value && !std::is_same<cpp::dynamic_reflection::Object, T>::value
    >>
    cpp::dynamic_reflection::Object get(const T& object) const
    {
        return get(&object);
    }

    template<typename T, typename = std::enable_if_t<
        !std::is_same<void*, T>::value && !std::is_same<cpp::dynamic_reflection::Object, T>::value
    >>
    cpp::dynamic_reflection::Object get(T& object)
    {
        return get(&object);
    }

    cpp::dynamic_reflection::Object get(const void* object) const
    {
        return {_type, reinterpret_cast<char*>(const_cast<void*>(object)) + _offset};
    }

    cpp::dynamic_reflection::Object get(void* object)
    {
        return {_type, reinterpret_cast<char*>(object) + _offset, cpp::dynamic_reflection::Object::ConstructReference};
    }

    cpp::dynamic_reflection::Object get(const cpp::dynamic_reflection::Object& object) const
    {
        return get(object.raw());
    }

    cpp::dynamic_reflection::Object get(cpp::dynamic_reflection::Object& object)
    {
        return get(object.raw());
    }

    const cpp::dynamic_reflection::Type& type() const
    {
        return _type;
    }

    std::size_t offset() const
    {
        return _offset;
    }

private:
    cpp::dynamic_reflection::Type _type;
    cpp::dynamic_reflection::Type _declType;
    std::string _name;
    std::size_t _offset;
};

template<typename T>
class BindedField : public ::cpp::dynamic_reflection::Field
{
public:
    BindedField(const Field& field, const T& object) :
        Field{field},
        _object{const_cast<T*>(&object)}
    {}

    cpp::dynamic_reflection::Object get() const
    {
        return Field::get(*_object);
    }

    cpp::dynamic_reflection::Object get()
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
}

#endif // SIPLASPLAS_REFLECTION_FIELD_HPP
