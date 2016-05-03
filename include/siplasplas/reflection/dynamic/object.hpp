#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP

#include "type.hpp"

#include <memory>
#include <cassert>

namespace cpp
{

namespace dynamic_reflection
{

namespace detail
{
    class ObjectDeleter
    {
    public:
        ObjectDeleter(const cpp::dynamic_reflection::Type& type, bool isReference = false) :
            _type{type},
            _isReference{isReference}
        {}

        void operator()(void* object) const
        {
            if(!_isReference)
            {
                _type.destroy(object);
            }
        }

    private:
        const cpp::dynamic_reflection::Type _type;
        const bool _isReference;
    };
}

class Object
{
public:
    template<typename T>
    static void registerObject()
    {
        cpp::dynamic_reflection::Type::registerType<T>();
    }

    static constexpr bool ConstructReference = true;

    Object(const cpp::dynamic_reflection::Type& type) :
        _object{type.create(), detail::ObjectDeleter{type}},
        _type{type}
    {}

    Object(const cpp::dynamic_reflection::Type& type, void* fromRaw, bool isReference = false) :
        _object{(isReference ? fromRaw : type.create(fromRaw)), detail::ObjectDeleter{type, isReference}},
        _type{type},
        _isReference{isReference}
    {}

    template<typename T>
    Object(const T& value) :
        _object{cpp::dynamic_reflection::Type::get<T>().create(&value), detail::ObjectDeleter{cpp::dynamic_reflection::Type::get<T>()}},
        _type{cpp::dynamic_reflection::Type::get<T>()}
    {}

    template<typename T>
    Object& operator=(const T& value)
    {
        get<T>() = value;
        return *this;
    }

    template<typename T>
    const T& get() const
    {
        assert(ctti::type_id<T>() == _type.type().type_id());
        return *reinterpret_cast<const T*>(_object.get());
    }

    template<typename T>
    T& get()
    {
        assert(ctti::type_id<T>() == _type.type().type_id());
        return *reinterpret_cast<T*>(_object.get());
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
        return _object.get();
    }

    void* raw()
    {
        return _object.get();
    }

    Object& operator=(const Object& other)
    {
        assert(type().type().type_id() == other.type().type().type_id());

        _type.assign(_object.get(), other._object.get());

        return *this;
    }

    std::string toString() const
    {
        return _type.toString(_object.get());
    }

    static Object fromString(const std::string& typeName, const std::string& value)
    {
        auto type = cpp::dynamic_reflection::Type::get(typeName);
        return Object{type, type.fromString(value)};
    }

private:
    std::shared_ptr<void> _object;
    cpp::dynamic_reflection::Type _type;
    bool _isReference = false;
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_HPP
