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
    template<typename T>
    static void registerObject()
    {
        cpp::dynamic_reflection::Type::registerType<T>();
    }

    struct Empty {};

    static constexpr bool ConstructReference = true;

    Object() :
        Object{Type::get<Empty>()}
    {}

    Object(const cpp::dynamic_reflection::Type& type) :
        _type{type},
        _object{_type.construct()},
        _isReference{false}
    {}

    Object(const cpp::dynamic_reflection::Type& type, void* fromRaw, bool isReference = false) :
        _type{type},
        _object{isReference ? fromRaw : _type.copy_construct(fromRaw)},
        _isReference{isReference}
    {}

    template<typename T>
    Object(const T& value) :
        _type{cpp::dynamic_reflection::Type::get<T>()},
        _object{cpp::dynamic_reflection::Type::get<T>().copy_construct(&value)},
        _isReference{false}
    {}

    Object(const Object& other) :
        _type{other._type},
        _object{other._isReference ? other._object : _type.copy_construct(other._object)},
        _isReference{other._isReference}
    {}

    Object(Object&& other) :
        _type{other._type},
        _object{other._isReference ? other._object : _type.move_construct(other._object)},
        _isReference{other._isReference}
    {}

    Object& operator=(const Object& other)
    {
        if(_type != other._type)
        {
            destroy();
            _type = other._type;
            _object = other._isReference ? other._object : _type.copy_construct(other._object);
            _isReference = other._isReference;
        }
        else
        {
            _type.copy_assign(_object, other._object);
        }

        return *this;
    }

    Object& operator=(Object&& other)
    {
        if(_type != other._type)
        {
            destroy();
            _type = other._type;
            _object = other._isReference ? other._object : _type.move_construct(other._object);
            _isReference = other._isReference;
        }
        else
        {
            _type.move_assign(_object, other._object);
        }

        return *this;
    }

    ~Object()
    {
        destroy();
    }

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

    cpp::dynamic_reflection::Type type() const
    {
        return _type;
    }

    bool isReference() const
    {
        return _isReference;
    }

    const void* raw() const
    {
        return _object;
    }

    void* raw()
    {
        return _object;
    }

    std::string toString() const
    {
        return _type.toString(_object);
    }

    static Object fromString(const std::string& typeName, const std::string& value)
    {
        auto type = cpp::dynamic_reflection::Type::get(typeName);
        return Object{type, type.fromString(value)};
    }

private:
    cpp::dynamic_reflection::Type _type;
    void* _object;
    bool _isReference = false;

    void destroy()
    {
        if(!_isReference)
        {
            _type.destroy(_object);
        }
    }
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
