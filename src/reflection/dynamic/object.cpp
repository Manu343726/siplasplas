#include "object.hpp"

using namespace cpp;
using namespace cpp::dynamic_reflection;

struct EmptyObject {};

Object::Object() :
    Object{Type::get<EmptyObject>()}
{}

Object::Object(const cpp::dynamic_reflection::Type& type) :
    _type{type},
    _kind{Object::Kind::VALUE},
    _object{_type.construct()}
{}

Object::Object(const cpp::dynamic_reflection::Type& type, void* fromRaw, bool isReference) :
    _type{type},
    _kind{isReference ? Object::Kind::REFERENCE : Object::Kind::VALUE},
    _object{isReference ? fromRaw : _type.copy_construct(fromRaw)}
{}

Object::Object(const Object& other) :
    _type{other._type},
    _kind{other._kind},
    _object{other._kind != Object::Kind::VALUE ? other._object : _type.copy_construct(other._object)}
{}

Object::Object(Object&& other) :
    _type{other._type},
    _kind{other._kind},
    _object{other._kind != Object::Kind::VALUE ? other._object : _type.move_construct(other._object)}
{}

Object& Object::operator=(const Object& other)
{
    if(_type != other._type)
    {
        destroy();
        _type = other._type;
        _object = other._kind != Object::Kind::VALUE  ? other._object : _type.copy_construct(other._object);
        _kind = other._kind;
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
        _object = other._kind != Object::Kind::VALUE ? other._object : _type.move_construct(other._object);
        _kind = other._kind;
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
    return _kind == Object::Kind::REFERENCE;
}

Object::Kind Object::kind() const
{
    return _kind;
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
    if(!empty() && _kind == Object::Kind::VALUE)
    {
        _type.destroy(_object);
    }

    _object = nullptr;
    _type = Type::get<EmptyObject>();
}

