#ifndef SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
#define SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP

/*
 * Classes:
{% for class in classes %}
 * :: From line {{class.cursor.location.line}}: {{class.declarationKind}} {{class.className}} ({{class.fullname}})
{% for name, f in class.children['field'].iteritems() %}>
 *    -> field "{{f.displayname}}"
{% endfor %}
{% for name, f in class.children['method'].iteritems() %}
 *    -> method "{{f.displayname}}"
{% for attr in f.attributes %}
 *       @ Attribute: {{attr.code}}
{% endfor %}
{% endfor %}
{% endfor %}
 */

#include <ctti/type_id.hpp>

namespace cpp
{
{% for class in classes %}
    template<>
    class Reflection<ctti::type_id<{{class.fullname}}>().hash()>
    {
    public:
        static ::cpp::MetaClassData& reflection() {
            static ::cpp::MetaClassData& data = []() -> ::cpp::MetaClassData&
            {
                ::cpp::MetaClass::registerClass<{{class.fullname}}>({
{% for name, field in class.children['field'].iteritems() %}
                    ::cpp::Field("{{field.spelling}}", &{{class.fullname}}::{{field.spelling}}, offsetof({{class.fullname}}, {{field.spelling}})),
{% endfor %}
                }, {
{% for name, method in class.children['method'].iteritems() %}
{% if method.attribute is none %}
                    ::cpp::Function("{{method.spelling}}",  &{{class.fullname}}::{{method.spelling}}),
{% else %}
                    ::cpp::Function("{{method.spelling}}",  &{{class.fullname}}::{{method.spelling}}, {{method.attribute.code}}),
{% endif %}
{% endfor %}
                });

                return ::cpp::MetaClass::getClass<{{class.fullname}}>();
            }();

            return data;
        }
    };
{% endfor %}
}
#endif // SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
