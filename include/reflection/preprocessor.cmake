

function(reflection_generate_body CLASS FIELDS FUNCTIONS RESULT)
    
    foreach(FIELD ${FIELDS})
        set(field SIPLASPLAS_REFLECTION_FIELD(${CLASS}, ${FIELD}))

        if(fields)
            set(fields "${fields}, ${field}")
        else()
            set(fields ${field})
        endif()
    endforeach()

    foreach(FUNCTION ${FUNCTIONS})
        set(function SIPLASPLAS_REFLECTION_FUNCTION(${CLASS}, ${FUNCTION}))

        if(functions)
            set(functions "${fields}, ${field}")
        else()
            set(functions ${field})
        endif()
    endforeach()

    set(reflection_function_body 
"
public:
    static ::cpp::MetaClassData& reflection()
    {
        static ::cpp::MetaClassData& data = [] -> ::cpp::MetaClassData&
        {
            ::cpp::MetaClass::registerClass<${CLASS}>({
                ${fields}
            }, {
                ${functions}
            });

            return ::cpp::MetaClass::getClass<${CLASS}>();
        };

        return data;
    } 
")

    set(${RESULT} "${reflection_function_body}" PARENT_SCOPE)
endfunction()