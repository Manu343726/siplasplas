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

{% import 'static_reflection.jinja2' as static_reflection_macros %}

{% for class in classes %}
{{static_reflection_macros.codegen_class(class)}}
{% endfor %}
#endif // SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
