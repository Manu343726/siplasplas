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

    if(PythonPip_DEBUG)
        message(STATUS "Trying to find \"${NAME}\" program...")
    endif()

    find_program(pip NAMES ${NAME})

    if(pip)
        # get found pip version:
        execute_process(COMMAND ${pip} --version OUTPUT_VARIABLE pip_version)
        string(STRIP "${pip_version}" pip_version)

        # Parse the python version number:
        # The format is "pip <pip version> from <path> (python <python version>)"
        # example: pip 8.1.2 from /usr/lib/python2.7/site-packages (python 2.7)

        string(REGEX REPLACE ".*\\(python ([0-9]+)\\.[0-9]+.*\\)" "\\1" pip_python_version_major "${pip_version}")
        string(REGEX REPLACE ".*\\(python [0-9]+\\.([0-9])+.*\\)" "\\1" pip_python_version_minor "${pip_version}")
        string(STRIP "${pip_python_version_major}" _PYTHON_PIP_PYTHON_VERSION_MAJOR)
        string(STRIP "${pip_python_version_minor}" _PYTHON_PIP_PYTHON_VERSION_MINOR)
        set(_PYTHON_PIP_PYTHON_VERSION_STRING "${_PYTHON_PIP_PYTHON_VERSION_MAJOR}.${_PYTHON_PIP_PYTHON_VERSION_MINOR}")

        set(_PYTHON_PIP_EXECUTABLE "${pip}")

        if(PythonPip_DEBUG)
            message(STATUS "Found:")
            message(STATUS "  Executable: ${pip}")
            message(STATUS "  Version (full):   ${pip_version}")
            message(STATUS "  Version (string): ${_PYTHON_PIP_PYTHON_VERSION_STRING}")
            message(STATUS "  Version (major):  ${_PYTHON_PIP_PYTHON_VERSION_MAJOR}")
            message(STATUS "  Version (minor):  ${_PYTHON_PIP_PYTHON_VERSION_MINOR}")
        endif()
    endif()
endmacro()

macro(_find_pip)
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
endmacro()

if(_CACHE_PYTHON_PIP_EXECUTABLE)
    # If find_package(PythonPip) was already called but with a different version number,
    # rerun the search:
    if(PythonPip_FIND_VERSION)
        if(NOT (PythonPip_FIND_VERSION VERSION_EQUAL _CACHE_PYTHON_PIP_PYTHON_VERSION_STRING))
            if(PythonPip_DEBUG)
                message(STATUS "Previous pip python version (${_CACHE_PYTHON_PIP_PYTHON_VERSION_STRING}) does not match required version (${PythonPip_FIND_VERSION}). Running search again...")
            endif()
            _find_pip()
        else()
            if(PythonPip_DEBUG)
                message(STATUS "Previous pip python version (${_CACHE_PYTHON_PIP_PYTHON_VERSION_STRING}) matches required version (${PythonPip_FIND_VERSION}). Reading values from cache...")
            endif()

            # Load results from cache:
            set(PYTHON_PIP_EXECUTABLE "${_CACHE_PYTHON_PIP_EXECUTABLE}")
            set(PYTHON_PIP_PYTHON_VERSION_STRING "${_CACHE_PYTHON_PIP_PYTHON_VERSION_STRING}")
            set(PYTHON_PIP_PYTHON_VERSION_MAJOR  "${_CACHE_PYTHON_PIP_PYTHON_VERSION_MAJOR}")
            set(PYTHON_PIP_PYTHON_VERSION_MINOR  "${_CACHE_PYTHON_PIP_PYTHON_VERSION_MINOR}")
        endif()
    endif()
else()
    # If find_package(PythonPip) was not run before, run search:

    if(PythonPip_DEBUG)
        message(STATUS "Running pip search for first time")
    endif()
    _find_pip()
endif()

# handle the QUIETLY AND REQUIRED standard arguments and set PYTHONPIP_FOUND to true if
# PYTHON_PIP_EXECUTABLE is set
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PythonPip
REQUIRED_VARS
    PYTHON_PIP_EXECUTABLE
VERSION_VAR
    PYTHON_PIP_PYTHON_VERSION_STRING
)

if(PythonPip_FOUND)
    if(PythonPip_DEBUG)
        message(STATUS "pip found. Caching results...")
    endif()

    # Cache results so we can check in later invocations if
    # the search (which could be slow) has to be run again:
    set(_CACHE_PYTHON_PIP_EXECUTABLE "${PYTHON_PIP_EXECUTABLE}" CACHE INTERNAL "Python pip executable path")
    set(_CACHE_PYTHON_PIP_PYTHON_VERSION_STRING "${PYTHON_PIP_PYTHON_VERSION_STRING}" CACHE INTERNAL "pip python version (string)")
    set(_CACHE_PYTHON_PIP_PYTHON_VERSION_MAJOR  "${PYTHON_PIP_PYTHON_VERSION_MAJOR}"  CACHE INTERNAL "pip python version (major)")
    set(_CACHE_PYTHON_PIP_PYTHON_VERSION_MINOR  "${PYTHON_PIP_PYTHON_VERSION_MINOR}"  CACHE INTERNAL "pip python version (minor)")
endif()

if(PythonPip_DEBUG)
    message(STATUS "End of FindPythonPip.cmake script")
endif()
