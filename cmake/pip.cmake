
find_package(PythonPip ${SIPLASPLAS_DRLPARSER_PYTHON_VERSION} REQUIRED)

function(pip_package_version PACKAGE RESULT)
    unset(${RESULT} PARENT_SCOPE)

    execute_process(
        COMMAND ${PYTHON_PIP_EXECUTABLE} show ${PACKAGE}
        RESULT_VARIABLE result_code
        OUTPUT_VARIABLE stdout
    )

    if(result_code EQUAL 0)
        string(REGEX REPLACE "\n" ";" stdout "${stdout}")

        foreach(line ${stdout})
            # Ignore --- header
            if(line MATCHES "(\\-+)")
                continue()
            endif()

            string(REGEX REPLACE "(.*): .*" "\\1" key   "${line}")
            string(REGEX REPLACE ".*: (.*)" "\\1" value "${line}")

            if(key STREQUAL "Version")
                set(${RESULT} "${value}" PARENT_SCOPE)
                return()
            endif()
        endforeach()
    endif()
endfunction()

function(pip_install_package PACKAGE VERSION)
    if(VERSION)
        set(package "${PACKAGE}==${VERSION}")
    else()
        set(package ${PACKAGE})
    endif()

    execute_process(
        COMMAND ${PYTHON_PIP_EXECUTABLE} install --user ${package}
    )
endfunction()

function(pip_install_requirements PATH_TO_REQUIREMENTS_FILE)
    execute_process(
        COMMAND ${PYTHON_PIP_EXECUTABLE} install --user -r ${PATH_TO_REQUIREMENTS_FILE}/requirements.txt
    )
endfunction()
