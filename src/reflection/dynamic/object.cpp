#include "object.hpp"

using namespace cpp;
using namespace cpp::dynamic_reflection;

struct EmptyObject {};

Object::Object() :
    Object{Type::get<EmptyObject>()}
{}

Object::Object(const cpp::dynamic_reflection::Type& type) :
    _type{type},
    _isReference{false},
    _object{_type.construct()}
{}

Object::Object(const cpp::dynamic_reflection::Type& type, void* fromRaw, bool isReference) :
    _type{type},
    _isReference{isReference},
    _object{isReference ? fromRaw : _type.copy_construct(fromRaw)}
{}

Object::Object(const Object& other) :
    _type{other._type},
    _isReference{other._isReference},
    _object{other._isReference ? other._object : _type.copy_construct(other._object)}
{}

Object::Object(Object&& other) :
    _type{other._type},
    _isReference{other._isReference},
    _object{other._isReference ? other._object : _type.move_construct(other._object)}
{}

Object& Object::operator=(const Object& other)
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

Object& Object::operator=(Object&& other)
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

Object::~Object()
{
    destroy();
}

Type Object::type() const
{
    return _type;
}

bool Object::isReference() const
{
    return _isReference;
}

bool Object::empty() const
{
    return _type == Type::get<EmptyObject>();
}

const void* Object::raw() const
{
    return _object;
}

void* Object::raw()
{
    return _object;
}

std::string Object::toString() const
{
    return _type.toString(_object);
}

Object Object::fromString(const std::string& typeName, const std::string& value)
{
    auto type = cpp::dynamic_reflection::Type::get(typeName);
    return Object{type, type.fromString(value)};
}

void Object::destroy()
{
    if(! _isReference)
    {
        _type.destroy(_object);
    }

    _type = Type::get<EmptyObject>();
}

