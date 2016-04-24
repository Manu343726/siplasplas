#ifndef SIPLASPLAS_REFLECTION_METACLASS_HPP
#define SIPLASPLAS_REFLECTION_METACLASS_HPP

#include "field.hpp"
#include "function.hpp"

#include <initializer_list>

namespace cpp
{
    template<typename T>
    class Reflectable
    {

    };

    class MetaClassData
    {
    public:
        class DefaultConstructedMetaClassDataType {};

        MetaClassData() :
            _type{MetaType::get<DefaultConstructedMetaClassDataType>()}
        {}

        MetaClassData(const cpp::MetaType& type) :
            _type{type}
        {}

        MetaClassData(const cpp::MetaType& type, const std::initializer_list<cpp::Field>& fields, const std::initializer_list<cpp::Function>& functions)
        : _type{type}
        {
            for(const cpp::Field& field : fields)
            {
                _fields[field.name()] = field;
            }

            for (const cpp::Function& function : functions)
            {
                _functions[function.name()] = function;
            }
        }

        const cpp::Field& field(const std::string& name) const
        {
            return _fields.at(name);
        }

        const cpp::Function& function(const std::string& name) const
        {
            return _functions.at(name);
        }

        cpp::Field& field(const std::string& name)
        {
            return _fields.at(name);
        }

        cpp::Function& function(const std::string& name)
        {
            return _functions.at(name);
        }

        std::unordered_map<std::string, cpp::Field>& fields()
        {
            return _fields;
        }

        std::unordered_map<std::string, cpp::Function>& functions()
        {
            return _functions;
        }

        const std::unordered_map<std::string, cpp::Field>& fields() const
        {
            return _fields;
        }

        const std::unordered_map<std::string, cpp::Function>& functions() const
        {
            return _functions;
        }


        const cpp::MetaType& type() const
        {
            return _type;
        }

    private:
        std::unordered_map<std::string, cpp::Field> _fields;
        std::unordered_map<std::string, cpp::Function> _functions;
        cpp::MetaType _type;
    };

    template<typename T>
    class BindedMetaClassData
    {
    public:
        BindedMetaClassData(const T* object, const MetaClassData* data) :
            _object{const_cast<T*>(object)},
            _data{data}
        {
            assert(data->type() == MetaType::get<T>());
        }

        cpp::BindedField<T> field(const std::string& name) const
        {
            return { _data->field(name), *_object};
        }

        cpp::BindedFunction<T> function(const std::string& name) const
        {
            return { _data->function(name), *_object};
        }

    private:
        T* _object;
        const MetaClassData* _data;
    };

    class MetaClass
    {
    public:
        struct UnknownClass {};

        template<typename T>
        static void registerClass(const cpp::MetaType& type, const std::initializer_list<cpp::Field>& fields, const std::initializer_list<cpp::Function>& functions)
        {
            metaClasses()[ctti::unnamed_type_id<T>()] = MetaClassData{type, fields, functions};
        }

        static MetaClassData& getClass(const ctti::unnamed_type_id_t id)
        {
            auto it = metaClasses().find(id);

            if(it != metaClasses().end())
            {
                return it->second;
            }
            else
            {
                // Returns class with no members info,
                // works for basic types and aggregates
                // where reflection is not needed (Else they
                // would be registered anyway)
                static MetaClassData unknownClass{MetaType::get<UnknownClass>()};
                return unknownClass;
            }
        }

        template<typename Class>
        static MetaClassData& getClass()
        {
            return getClass(ctti::unnamed_type_id<Class>());
        }

        static MetaClassData& getClass(const cpp::TypeInfo& typeInfo)
        {
            return getClass(typeInfo.name());
        }

        static MetaClassData& getClass(const cpp::MetaType& type)
        {
            return getClass(type.typeInfo());
        }

        static MetaClassData& getClass(const std::string& className)
        {
            return getClass(ctti::id_from_name(className));
        }

        static const std::unordered_map<ctti::type_index, MetaClassData>& getClasses()
        {
            return metaClasses();
        }

    protected:
        using MetaClassRegistry = std::unordered_map<ctti::type_index, MetaClassData>;
        static MetaClassRegistry& metaClasses();
    };

    
    template<typename T>
    class Reflection
    {
    public:
        static ::cpp::MetaClassData& reflection()
        {
            static MetaClassData& data = []() -> MetaClassData&
            {
                // Return class data with no members info
                static MetaClassData noRegisteredClass{MetaType::get<T>()};
                return noRegisteredClass;
            }();

            return data;
        }
    };
}

#endif // SIPLASPLAS_REFLECTION_METACLASS_HPP
