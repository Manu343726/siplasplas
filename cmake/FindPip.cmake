# FindPythonPip module
# --------------------
#
# Find python pip dependency manager
#
# Output variables:
#
#   PYTHON_PIP_EXECUTABLE               Full path to pip executable
#   PYTHON_PIP_PYTHON_VERSION_STRING    Python version of found pip e.g. 2.7
#   PYTHON_PIP_PYTHON_VERSION_MAJOR     Python major version of found pip e.g. 2
#   PYTHON_PIP_PYTHON_VERSION_MINOR     Python minor version of found pip e.g. 7

macro(_find_and_check_pip NAME)
    unset(_PYTHON_PIP_EXECUTABLE)
    unset(_PYTHON_PIP_PYTHON_VERSION_STRING)
    unset(_PYTHON_PIP_PYTHON_VERSION_MAJOR)
    unset(_PYTHON_PIP_PYTHON_VERSION_MINOR)

    find_program(pip NAMES ${NAME})

    if(pip)
        # get found pip version:

        execute_process(COMMAND ${pip} --version OUTPUT_VARIABLE pip_version)

        # Parse the python version number:
        # The format is "pip <pip version> from <path> (python <python version>)"
        # example: pip 8.1.2 from /usr/lib/python2.7/site-packages (python 2.7)

        string(REGEX REPLACE ".*\\(python ([0-9]+)\\.[0-9]+.*\\)" "\\1" pip_python_version_major "${pip_version}")
        string(REGEX REPLACE ".*\\(python [0-9]+\\.([0-9])+.*\\)" "\\1" pip_python_version_minor "${pip_version}")
        string(STRIP "${pip_python_version_major}" _PYTHON_PIP_PYTHON_VERSION_MAJOR)
        string(STRIP "${pip_python_version_minor}" _PYTHON_PIP_PYTHON_VERSION_MINOR)
        set(_PYTHON_PIP_PYTHON_VERSION_STRING "${PYTHON_PIP_PYTHON_VERSION_MAJOR}.${PYTHON_PIP_PYTHON_VERSION_MINOR}")

        set(_PYTHON_PIP_EXECUTABLE "${pip}")
    endif()
endmacro()

set(_PIP_NAMES pip2 pip)

foreach(name ${_PIP_NAMES})
    _find_and_check_pip(${name})

    if(_PYTHON_PIP_EXECUTABLE)
        if(PythonPip_FIND_VERSION)
            if(_PYTHON_PIP_PYTHON_VERSION_STRING VERSION_EQUAL PythonPip_FIND_VERSION)
                set(_VALID_PIP TRUE)
            else()
                set(_VALID_PIP FALSE)
            endif()
        else()
            set(_VALID_PIP TRUE)
        endif()

        if(_VALID_PIP)
            set(PYTHON_PIP_EXECUTABLE            "${_PYTHON_PIP_EXECUTABLE}")
            set(PYTHON_PIP_PYTHON_VERSION_STRING "${_PYTHON_PIP_PYTHON_VERSION_STRING}")
            set(PYTHON_PIP_PYTHON_VERSION_MAJOR  "${_PYTHON_PIP_PYTHON_VERSION_MAJOR}")
            set(PYTHON_PIP_PYTHON_VERSION_MINOR  "${_PYTHON_PIP_PYTHON_VERSION_MINOR}")
            break()
        endif()
    endif()
endforeach()

# handle the QUIETLY AND REQUIRED standard arguments and set PYTHONPIP_FOUND to true if
# PYTHON_PIP_EXECUTABLE is set
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PythonPip
REQUIRED_VARS
    PYTHON_PIP_EXECUTABLE
VERSION_VAR
    PYTHON_PIP_PYTHON_VERSION_STRING
)

mark_as_advanced(PYTHON_PIP_EXECUTABLE)
