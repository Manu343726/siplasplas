#ifndef SIPLASPLAS_REFLECTION_METACLASS_HPP
#define SIPLASPLAS_REFLECTION_METACLASS_HPP

#include "reflection/field.hpp"
#include "reflection/function.hpp"

#include <initializer_list>

namespace cpp
{
    class MetaClassData
    {
    public:
        MetaClassData() = default;

        MetaClassData(const std::initializer_list<cpp::Field>& fields, const std::initializer_list<cpp::Function>& functions)
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

        cpp::Field field(const std::string& name) const
        {
            return _fields.at(name);
        }

        cpp::Function function(const std::string& name) const
        {
            return _functions.at(name);
        }

        const std::unordered_map<std::string, cpp::Field>& fields() const
        {
            return _fields;
        }

        const std::unordered_map<std::string, cpp::Function>& functions() const
        {
            return _functions;
        }

    private:
        std::unordered_map<std::string, cpp::Field> _fields;
        std::unordered_map<std::string, cpp::Function> _functions;
    };

    template<typename T>
    class BindedMetaClassData
    {
    public:
        BindedMetaClassData(const T& object, const MetaClassData& data) :
            _object{const_cast<T*>(&object)},
            _data{&data}
        {}
    
        cpp::Field::Binded<T> field(const std::string& name) const
        {
            return _data->field(name).bind(*_object);
        }

        cpp::Function::Binded<T> function(const std::string& name) const
        {
            return _data->function(name).bind(*_object);
        }

    private:
        T* _object;
        const MetaClassData* _data;
    };

    class MetaClass
    {
    public:
        template<typename T>
        static void registerClass(const std::initializer_list<cpp::Field>& fields, const std::initializer_list<cpp::Function>& functions)
        {
            _metaClasses[ctti::unnamed_type_id<T>()] = MetaClassData{fields, functions};
        }

        static MetaClassData& getClass(const ctti::unnamed_type_id_t id)
        {
            return _metaClasses.at(id);
        }

        template<typename Class>
        static MetaClassData& getClass()
        {
            return getClass(ctti::unnamed_type_id<Class>());
        }

        static MetaClassData& getClass(const char* className)
        {
            return getClass(ctti::id_from_name(className));
        }

        static MetaClassData& getClass(const std::string& className)
        {
            return getClass(ctti::id_from_name(className));
        }

    protected:
        using MetaClassRegistry = std::unordered_map<ctti::type_index, MetaClassData>;
        static MetaClassRegistry _metaClasses;
    };

    MetaClass::MetaClassRegistry MetaClass::_metaClasses;

    template<typename Class>
    class MetaClassFor : public MetaClass
    {
    public:
        static MetaClassData& reflection()
        {
            return _metaClasses.at(ctti::unnamed_type_id<Class>());
        }

        BindedMetaClassData<Class> instanceReflection() const
        {
            return { static_cast<const Class&>(*this), reflection() };
        }
    };

#define SIPLASPLAS_ENABLE_REFLECTION struct EnableReflection {};

}

#endif // SIPLASPLAS_REFLECTION_METACLASS_HPP
