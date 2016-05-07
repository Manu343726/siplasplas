#ifndef SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
#define SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP

#include <ctti/type_id.hpp>
#include <siplasplas/reflection/api.hpp>

{% import 'static_reflection_codegen.jinja2' as codegen %}

{{codegen.namespace(global_namespace)}}
#endif // SIPLASPLAS_REFLECTION_OUTPUT_FILE_{{hash}}_HPP
