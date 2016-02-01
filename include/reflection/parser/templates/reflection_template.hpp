#ifndef SIPLASPLAS_REFLECTION_INIT_FILE_{{hash}}_HPP
#define SIPLASPLAS_REFLECTION_INIT_FILE_{{hash}}_HPP

/*
 * Reflection initialization file generated from file:
 * {{tu.filePath}}
 *
 * Classes:
{% for class in tu.classes %}
 * - From line {{class.cursor.location.line}}: {{class.declKind}} {{class.className}} ({{class.full_qualified_ref}})
{% endfor %}
 */ 

namespace cpp
{
{% for class in tu.classes %}
    template<>
    class Reflection<{{class.full_qualified_ref}}>
    {
    public:
        static ::cpp::MetaClassData& reflection() {
            static ::cpp::MetaClassData& data = []() -> ::cpp::MetaClassData& 
            {
                ::cpp::MetaClass::registerClass<{{class.full_qualified_ref}}>({
                    {% for field in class.fields %}
                        ::cpp::Field("{{field.name}}", &{{class.full_qualified_ref}}::{{field.name}}, offsetof({{class.full_qualified_ref}}, {{field.name}})),
                    {% endfor %}
                }, { 
                    {% for function in class.functions %}
                        ::cpp::Function("{{function.name}}",  &{{class.full_qualified_ref}}::{{function.name}}),
                    {% endfor %}
                });

                return ::cpp::MetaClass::getClass<{{class.className}}>();
            }();

            return data;
        }
    };

{% endfor %}
}
#endif #define SIPLASPLAS_REFLECTION_INIT_FILE_{{hash}}_HPP