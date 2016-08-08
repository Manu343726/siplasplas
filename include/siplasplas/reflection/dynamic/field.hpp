#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_FIELD_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_FIELD_HPP

#include "object.hpp"
#include "entity.hpp"

#include <type_traits>
#include <string>
#include <cstddef>

namespace cpp
{

namespace dynamic_reflection
{

class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT Field : public Entity
{
public:

    Object get(const void* object) const;
    Object get(void* object);

    template<typename T, typename = std::enable_if_t<
        !std::is_same<void*, T>::value && !std::is_same<cpp::dynamic_reflection::Object, T>::value
    >>
    Object get(const T& object) const
    {
        return get(static_cast<const void*>(&object));
    }

    template<typename T, typename = std::enable_if_t<
        !std::is_same<void*, T>::value && !std::is_same<cpp::dynamic_reflection::Object, T>::value
    >>
    Object get(T& object)
    {
        return get(static_cast<void*>(&object));
    }

    Object get(const Object& object);
    Object get(Object& object);

    const Type& type() const;
    const Type& declType() const;
    bool isReference() const;

    template<typename Class, typename T>
    static std::shared_ptr<Field> create(const SourceInfo& sourceInfo, T Class::* field)
    {
        return std::shared_ptr<Field>{ new Field{sourceInfo, field} };
    }

    static Field& fromEntity(const std::shared_ptr<Entity>& entity);

private:
    template<typename Class, typename T>
    Field(const SourceInfo& sourceInfo, T Class::* field) :
        Entity{sourceInfo},
        _fieldAccess{new FieldAccess<T Class::*>{field}}
    {}

    class SIPLASPLAS_REFLECTION_DYNAMIC_EXPORT FieldAccessInterface
    {
    public:
        virtual ~FieldAccessInterface() = default;

        virtual Object get(void* object) = 0;
        virtual Object get(const void*) const = 0;
        virtual const Type& type() const = 0;
        virtual const Type& declType() const = 0;
        virtual bool isReference() const = 0;
    };

    std::unique_ptr<FieldAccessInterface> _fieldAccess;

    template<typename FieldType>
    class FieldAccess;

    template<typename Class, typename T>
    class FieldAccess<T Class::*> : public FieldAccessInterface
    {
    public:
        FieldAccess(T Class::* field) :
            _field{field},
            _type{Type::get<std::decay_t<T>>()},
            _declType{Type::get<T>()}
        {}

        bool isReference() const override
        {
            return std::is_reference<T>();
        }

        Object get(void* object) override
        {
            return {type(), &(reinterpret_cast<Class*>(object)->*_field), Object::ConstructReference};
        }

        Object get(const void* object) const override
        {
            return {type(), const_cast<T*>(&(reinterpret_cast<const Class*>(object)->*_field))};
        }

        const Type& type() const override
        {
            return _type;
        }

        const Type& declType() const override
        {
            return _declType;
        }

    private:
        T Class::* _field;
        Type _type;
        Type _declType;
    };
};

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_FIELD_HPP
