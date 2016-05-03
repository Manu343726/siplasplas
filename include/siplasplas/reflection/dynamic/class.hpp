#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP

#include "field.hpp"
#include "function.hpp"
#include <siplasplas/reflection/static/class.hpp>

#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/fusion.hpp>

#include <vector>
#include <iostream>

namespace cpp
{

namespace dynamic_reflection
{

class Class
{
public:
    struct UnknownClass {};

    using Functions = std::unordered_map<std::string, cpp::dynamic_reflection::Function>;
    using Fields    = std::unordered_map<std::string, cpp::dynamic_reflection::Field>;

    template<typename T>
    static Class& registerClass(const Type& type, const std::vector<Field>& fields,
                                                  const std::vector<Function>& functions)
    {
        return classes()[ctti::unnamed_type_id<T>()] = Class{type, fields, functions};
    }

    template<typename T>
    static Class& registerClass()
    {
        // Performs automatic registration from static reflection metadata
        static ::cpp::dynamic_reflection::Class& data = []() -> ::cpp::dynamic_reflection::Class&
        {
            return registerClass<T>(
                dynamic_reflection::Type::get<T>(),
                cpp::fmap<dynamic_reflection::Field, typename static_reflection::Class<T>::Fields>(
                    [](auto type){
                        using Field = typename decltype(type)::type;

                        // Register used classes recursively
                        dynamic_reflection::Class::registerClass<typename Field::value_type>();

                        return dynamic_reflection::Field(
                            Field::spelling(),
                            Field::get(),
                            Field::offset()
                        );
                    }
                ),
                cpp::fmap<dynamic_reflection::Method, typename static_reflection::Class<T>::Methods>(
                    [](auto type){
                        using Method = typename decltype(type)::type;

                        return dynamic_reflection::Method(Method::spelling(), Method::get());
                    }
                )
            );
        }();

        return data;
    }

    static Class& get(const ctti::unnamed_type_id_t id)
    {
        auto it = classes().find(id);

        if(it != classes().end())
        {
            return it->second;
        }
        else
        {
            // Returns class with no members info,
            // works for basic types and aggregates
            // where reflection is not needed (Else they
            // would be registered anyway)
            static Class unknownClass{Type::get<UnknownClass>()};
            return unknownClass;
        }
    }

    template<typename T>
    static Class& get()
    {
        return registerClass<T>();
    }

    static Class& get(const cpp::TypeInfo& typeInfo)
    {
        return get(typeInfo.name());
    }

    static Class& get(const cpp::dynamic_reflection::Type& type)
    {
        return get(type.typeInfo());
    }

    static Class& get(const std::string& className)
    {
        return get(ctti::id_from_name(className));
    }

    static const std::unordered_map<ctti::type_index, Class>& getClasses()
    {
        return classes();
    }

    class DefaultConstructedClassType {};

    Class() :
        _type{Type::get<DefaultConstructedClassType>()}
    {}

    Class(const cpp::dynamic_reflection::Type& type) :
        _type{type}
    {}

    Class(const cpp::dynamic_reflection::Type& type, const std::vector<cpp::dynamic_reflection::Field>& fields,
                                                     const std::vector<cpp::dynamic_reflection::Function>& functions)
    : _type{type}
    {
        for(const cpp::dynamic_reflection::Field& field : fields)
        {
            _fields[field.name()] = field;
        }

        for (const cpp::dynamic_reflection::Function& function : functions)
        {
            _functions[function.name()] = function;
        }
    }

    const cpp::dynamic_reflection::Field& field(const std::string& name) const
    {
        return _fields.at(name);
    }

    const cpp::dynamic_reflection::Function& function(const std::string& name) const
    {
        return _functions.at(name);
    }

    cpp::dynamic_reflection::Field& field(const std::string& name)
    {
        return _fields.at(name);
    }

    cpp::dynamic_reflection::Function& function(const std::string& name)
    {
        return _functions.at(name);
    }

    Fields& fields()
    {
        return _fields;
    }

    Functions& functions()
    {
        return _functions;
    }

    const Fields& fields() const
    {
        return _fields;
    }

    const Functions& functions() const
    {
        return _functions;
    }

    const cpp::dynamic_reflection::Type& type() const
    {
        return _type;
    }

protected:
    using Classes = std::unordered_map<ctti::type_index, Class>;
    static Classes& classes();

private:
    Functions _functions;
    Fields _fields;
    cpp::dynamic_reflection::Type _type;
};

template<typename T>
class BindedClass
{
public:
    BindedClass(const T* object, const Class* data) :
        _object{const_cast<T*>(object)},
        _data{data}
    {
        assert(data->type() == Type::get<T>());
    }

    cpp::dynamic_reflection::BindedField<T> field(const std::string& name) const
    {
        return { _data->field(name), *_object};
    }

    cpp::dynamic_reflection::BindedFunction<T> function(const std::string& name) const
    {
        return { _data->function(name), *_object};
    }

private:
    T* _object;
    const Class* _data;
};

namespace codegen
{

template<typename T>
class Class
{
};

} // codegen

} // dynamic_reflection
} // cpp

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_CLASS_HPP
