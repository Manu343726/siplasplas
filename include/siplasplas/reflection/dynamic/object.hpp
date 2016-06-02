#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP

#include "type.hpp"

#include <memory>
#include <cassert>
#include <type_traits>

namespace cpp
{

namespace dynamic_reflection
{

class Object
{
public:
    static constexpr bool ConstructReference = true;

    Object();
    Object(const cpp::dynamic_reflection::Type& type);
    Object(const cpp::dynamic_reflection::Type& type, void* fromRaw, bool isReference = false);
    template<typename T, typename = std::enable_if_t<
        !std::is_same<std::decay_t<T>, cpp::dynamic_reflection::Type>::value &&
        !std::is_same<std::decay_t<T>, Object>::value
    >>
    Object(T&& value) :
        _type{cpp::dynamic_reflection::Type::get<std::remove_reference_t<T>>()},
        _isReference{false},
        _object{_isReference ? &value : _type.copy_construct(&value)}
    {}
    Object(const Object& other);
    Object(Object&& other);
    Object& operator=(const Object& other);
    Object& operator=(Object&& other);
    ~Object();

    template<typename T>
    Object& operator=(const T& value)
    {
        get<T>() = value;
        return *this;
    }

    template<typename T>
    const T& get() const
    {
        //assert(ctti::type_id<T>() == _type.type().type_id());
        return *reinterpret_cast<const std::decay_t<T>*>(_object);
    }

    template<typename T>
    T& get()
    {
        //assert(ctti::type_id<T>() == _type.type().type_id());
        return *reinterpret_cast<std::decay_t<T>*>(_object);
    }

    template<typename T>
    operator const T&() const
    {
        return get<T>();
    }

    template<typename T>
    operator T&()
    {
        return get<T>();
    }

    cpp::dynamic_reflection::Type type() const;
    bool isReference() const;
    bool empty() const;

    const void* raw() const;
    void* raw();

    std::string toString() const;
    static Object fromString(const std::string& typeName, const std::string& value);

private:
    cpp::dynamic_reflection::Type _type;
    bool _isReference = false;
    void* _object;

    void destroy();
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
