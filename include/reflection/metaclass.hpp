#ifndef SIPLASPLAS_REFLECTION_METACLASS_HPP
#define SIPLASPLAS_REFLECTION_METACLASS_HPP

#include "reflection/field.hpp"

#include <initializer_list>

namespace cpp
{
    class MetaClassData
    {
    public:
        MetaClassData() = default;

        MetaClassData(const std::initializer_list<cpp::Field>& fields)
        {
            for(const cpp::Field& field : fields)
            {
                _fields[field.name()] = field;
            }
        }

        cpp::Field field(const std::string& name) const
        {
            return _fields.at(name);
        }

        const std::unordered_map<std::string, cpp::Field>& fields() const
        {
            return _fields;
        }

    private:
        std::unordered_map<std::string, cpp::Field> _fields;
    };

    class MetaClass
    {
    public:
        template<typename T>
        static void registerClass(const std::initializer_list<cpp::Field>& fields)
        {
            _metaClasses[ctti::unnamed_type_id<T>()] = MetaClassData{fields};
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
    };
}

#endif // SIPLASPLAS_REFLECTION_METACLASS_HPP
