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

    enum class Kind
    {
        VALUE,
        REFERENCE,
        POINTER
    };

    Object();
    Object(const cpp::dynamic_reflection::Type& type);
    Object(const cpp::dynamic_reflection::Type& type, void* fromRaw, bool isReference = false);
    template<typename T, typename = std::enable_if_t<
        !std::is_same<std::decay_t<T>, cpp::dynamic_reflection::Type>::value &&
        !std::is_same<std::decay_t<T>, Object>::value &&
        !std::is_pointer<std::decay_t<T>>::value
    >>
    Object(T&& value) :
        _type{cpp::dynamic_reflection::Type::get<std::decay_t<T>>()},
        _kind{Kind::VALUE},
        _object{_type.copy_construct(&value)}
    {}
    template<typename T>
    Object(const T* pointer) :
        _type{cpp::dynamic_reflection::Type::get<T*>()},
        _kind{Kind::POINTER},
        _object{const_cast<T*>(pointer)}
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
    T& get() const
    {
        return *reinterpret_cast<std::remove_reference_t<T>*>(_object);
    }

    template<typename T>
    operator T&() const
    {
        return get<T>();
    }

    cpp::dynamic_reflection::Type type() const;
    bool isReference() const;
    Kind kind() const;
    bool empty() const;

    const void* raw() const;
    void* raw();

    std::string toString() const;
    static Object fromString(const std::string& typeName, const std::string& value);

private:
    cpp::dynamic_reflection::Type _type;
    Kind _kind;
    void* _object;

    void destroy();
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
