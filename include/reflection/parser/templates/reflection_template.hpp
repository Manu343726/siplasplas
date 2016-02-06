#ifndef SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
#define SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP

/*
 * Reflection file generated from:
 * {{tu.filePath}}
 *
 * Classes:
{% for class in tu.classes %}
 * :: From line {{class.cursor.location.line}}: {{class.declKind}} {{class.className}} ({{class.full_qualified_ref}})
{% for f in class.fields %}
 *    -> field "{{f.name}}" 
{% endfor %}
{% for f in class.functions %}
 *    -> function "{{f.name}}" 
{% endfor %}
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

                return ::cpp::MetaClass::getClass<{{class.full_qualified_ref}}>();
            }();

            return data;
        }
    };
{% endfor %}
}
#endif // SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP