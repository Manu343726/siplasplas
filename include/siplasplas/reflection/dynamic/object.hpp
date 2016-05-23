#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP

#include "type.hpp"

#include <memory>
#include <cassert>

namespace cpp
{

namespace dynamic_reflection
{

class Object
{
public:
    static constexpr bool ConstructReference = true;

    /// 
    Object();
    Object(const cpp::dynamic_reflection::Type& type);
    Object(const cpp::dynamic_reflection::Type& type, void* fromRaw, bool isReference = false);
    template<typename T>
    Object(const T& value) :
        _type{cpp::dynamic_reflection::Type::get<T>()},
        _object{cpp::dynamic_reflection::Type::get<T>().copy_construct(&value)},
        _isReference{false}
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
        return *reinterpret_cast<const T*>(_object);
    }

    template<typename T>
    T& get()
    {
        //assert(ctti::type_id<T>() == _type.type().type_id());
        return *reinterpret_cast<T*>(_object);
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
    void* _object;
    bool _isReference = false;

    void destroy();
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
