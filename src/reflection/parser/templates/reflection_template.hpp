#ifndef SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
#define SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP

/*
 * Classes:
{% for class in classes %}
 * :: From line {{class.cursor.location.line}}: {{class.declarationKind}} {{class.className}} ({{class.fullname}})
{% for name, f in class.children['field'].iteritems() %}
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
#include <siplasplas/reflection/api.hpp>

namespace cpp
{
{% for class in classes %}
    template<>
    class Reflection<{{class.fullname}}>
    {
    public:
        static ::cpp::MetaClassData& registerReflection() {
            static ::cpp::MetaClassData& data = []() -> ::cpp::MetaClassData&
            {
                ::cpp::MetaClass::registerClass<{{class.fullname}}>(::cpp::MetaType::get<{{class.fullname}}>(), {
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

        static ::cpp::MetaClassData& reflection()
        {
            return _reflectionData;
        }

    private:
        static ::cpp::MetaClassData& _reflectionData;
    };

    ::cpp::MetaClassData& Reflection<{{class.fullname}}>::_reflectionData = Reflection<{{class.fullname}}>::registerReflection();
{% endfor %}
}
#endif // SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
