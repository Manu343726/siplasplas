include(cmake/custom_properties)

find_package(PythonInterp 2.7 REQUIRED)

if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found")
else()
    message(STATUS "Python interpreter found: ${PYTHON_EXECUTABLE} (${PYTHON_VERSION_STRING})")
endif()

set(DRLPARSER_SCRIPT include/reflection/parser/DRLParser)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/reflection)
set(INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/)

function(clangxx_executable _ret)
    if(CMAKE_CXX_COMPILER_ID  MATCHES "Clang")
        set(${_ret} "${CMAKE_CXX_COMPILER}" PARENT_SCOPE)
    else()
        if(NOT clangxx)
            find_program(clangxx clang++)

            if(clangxx)
                message(STATUS "Found clang++ program: ${clangxx}")
                set(${_ret} "${clangxx}" PARENT_SCOPE)
            else()
                message(FATAL_ERROR "clang++ not found")
            endif()
        else()
            set(${_ret} "${clangxx}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

function(gxx_executable _ret)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(${_ret} "${CMAKE_CXX_COMPILER}" PARENT_SCOPE)
    else()
        if(NOT gxx)
            find_program(gxx g++)

            if(gxx)
                message(STATUS "Found g++ program: ${gxx}")
                set(${_ret} "${gxx}" PARENT_SCOPE)
            else()
                message(FATAL_ERROR "g++ not found")
            endif()
        else()
            set(${_ret} "${gxx}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

function(windows_path PATH RESULT_PATH)
    if(WIN32)
        if(MINGW)
            execute_process(
                    COMMAND ${CMAKE_COMMAND} -E echo ${PATH}
                    COMMAND xargs cmd //c echo # WTF cannot believe this worked the very first time
                    OUTPUT_VARIABLE path
            )
        elseif(CYGWIN)
            execute_process(
                    COMMAND ${CMAKE_COMMAND} -E echo ${PATH}
                    COMMAND xargs cygpath.exe --windows
                    OUTPUT_VARIABLE path
            )
        else()
            set(path "${PATH}")
        endif()

        set(${RESULT_PATH} "${path}" PARENT_SCOPE)
    else()
        message(FATAL_ERROR "No Windows platform!")
    endif()
endfunction()

function(clangxx_stdlibcpp_includes INCLUDES)
    clangxx_executable(clangxx)

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${clangxx} -std=c++11 -stdlib=libc++ -v -x c++ -E -
        OUTPUT_VARIABLE out
        ERROR_VARIABLE err
    )

    string(REGEX MATCH "
#include \"...\" search starts here:
#include <...> search starts here:
(.+)
End of search list."
            includes "${err}")

    string(REGEX REPLACE "\n" ";" includes "${CMAKE_MATCH_1}")
    list(REMOVE_DUPLICATES includes)

    foreach(includedir ${includes})
        if(WIN32)
            windows_path("${includedir}" includedir)
        endif()

        string(REGEX REPLACE "\ +$" "" includedir "${includedir}")
        string(REGEX REPLACE "\"(.*)\"" "\\1" includedir "${includedir}")
        string(REGEX REPLACE "\n" "" includedir "${includedir}")

        list(APPEND includedirs ${includedir})
    endforeach()

    if(NOT includedirs)
        message(FATAL_ERROR "No stdlibc++ include directory found")
    else()
        set(${INCLUDES} "${includedirs}" PARENT_SCOPE)
    endif()
endfunction()

# Adapted from https://github.com/biicode/boost
function(clangxx_version _ret)
    clangxx_executable(clangxx)

    execute_process( COMMAND ${clangxx} --version OUTPUT_VARIABLE version_string_full )
    string (REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" version_string ${version_string_full})

    set(${_ret} ${version_string} PARENT_SCOPE)
endfunction()

function(gxx_version _ret)
    gxx_executable(gxx)

    execute_process( COMMAND ${gxx} -dumpversion OUTPUT_VARIABLE version_string)
    string (REGEX REPLACE "([0-9])\\.([0-9])\\.([0-9])" "\\1.\\2.\\3" version_string ${version_string})
    string(STRIP ${version_string} version_string) #Remove extra newline character

    set(${_ret} "${version_string}" PARENT_SCOPE)
endfunction()

# Needed to solve this issue http://clang.llvm.org/docs/LibTooling.html#builtin-includes
function(libclang_include_dir _ret)
    clangxx_version(CLANG_VERSION)
    clangxx_executable(clangxx)

    if(WIN32)
        windows_path("${CLANG_EXEC}" CLANG_EXEC)
    endif()

    get_filename_component(CLANG_PATH "${CLANG_EXEC}" DIRECTORY)
    get_filename_component(CLANG_PATH "${CLANG_PATH}" REALPATH)

    set(${_ret} "${CLANG_PATH}/../lib/clang/${CLANG_VERSION}" PARENT_SCOPE)
endfunction()

function(reflection_target TARGET)
    function(log MESSAGE)
        if(SIPLASPLAS_VERBOSE_CONFIG)
            message(STATUS "[REFLECTION LOG] ${MESSAGE}")
        endif()
    endfunction()

    if(NOT (TARGET ${TARGET}))
        message(FATA_ERROR "Target ${TARGET} not found!")
    endif()

    get_target_property(SOURCES ${TARGET} SOURCES)
    get_target_property(INCLUDE_DIRS ${TARGET} INCLUDE_DIRECTORIES)
    get_target_property(INTERFACE_INCLUDE_DIRS ${TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(COMPILE_OPTIONS ${TARGET} COMPILE_OPTIONS)

    if(UNIX OR MINGW)
        clangxx_stdlibcpp_includes(STDLIBCPP_INCLUDES)
        libclang_include_dir(LIBCLANG_INCLUDE_DIR)

            foreach(include ${STDLIBCPP_INCLUDES})
                log("Extra stdlibc++ include: ${include}")
            endforeach()
            log("libclang include dir: ${LIBCLANG_INCLUDE_DIR}")

        list(APPEND EXTRA_LIBCLANG_INCLUDES ${STDLIBCPP_INCLUDES} ${LIBCLANG_INCLUDE_DIR})
    endif()

    if(NOT INTERFACE_INCLUDE_DIRS)
        set(INTERFACE_INCLUDE_DIRS)
    endif()

    target_include_directories(${TARGET}
        PUBLIC ${INCLUDE_OUTPUT_DIR}
    )

    list(APPEND INCLUDE_DIRS ${INTERFACE_INCLUDE_DIRS})
    
    log("Processing target ${TARGET}:")

    foreach(source ${SOURCES})
        if(MSVC)
            source_file_has_custom_property(is_sourcegroups_header "${source}" VS_SOURCEGROUPS_HEADER)

            if(is_sourcegroups_header)
                list(APPEND headers ${source})
                continue()
            endif()
        endif()

        log("  - ${source}")
    endforeach()

    log("Setting preprocessor hook for target ${TARGET}")
    add_custom_target(${TARGET}_prebuild)
    add_dependencies(${TARGET} ${TARGET}_prebuild)
    string(REGEX REPLACE ";" " " SOURCES "${SOURCES}")
    string(REGEX REPLACE ";" " " INCLUDE_DIRS "${INCLUDE_DIRS}")

    if(EXTRA_LIBCLANG_INCLUDES)
        string(REGEX REPLACE ";" " " EXTRA_LIBCLANG_INCLUDES "${EXTRA_LIBCLANG_INCLUDES}")
        set(includedirs --includedirs ${EXTRA_LIBCLANG_INCLUDES})
    endif()

    if(DRLPARSER_DATABASE)
        log("DRLParser custom database file: ${DRLPARSER_DATABASE}")
        set(database --database ${DRLPARSER_DATABASE})
    else()
        set(database --database ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.json)
    endif()

    if(DRLPARSER_IGNORE_DATABASE)
        log("DRLParser ignore database")
        set(ignore_database --ignore-database)
    endif()

    if(DRLPARSER_LIBCLANG)
        log("DRLParser custom libclang file: ${DRLPARSER_LIBCLANG}")
        set(libclang --libclang ${DRLPARSER_LIBCLANG})
    endif()

    if(DRLPARSER_VERBOSE)
        set(verbose --verbose)
    endif()

    if(DRLPARSER_ASTDUMP)
        set(astdump --ast-dump)
    endif()

    string(REGEX REPLACE ";" " " COMPILE_OPTIONS "${COMPILE_OPTIONS}")

    set(options
        --compile-options \"${COMPILE_OPTIONS}\"
        --searchdirs ${INCLUDE_DIRS}
        ${includedirs}
        -s ${CMAKE_SOURCE_DIR}
        -o ${OUTPUT_DIR}
        -x *3rdParty*
        ${database}
        ${libclang}
        ${ignore_database}
        ${verbose}
        ${astdump}
        --code-template-file ${CMAKE_SOURCE_DIR}/include/reflection/parser/templates/reflection_template.hpp
    )

    add_custom_command(
        TARGET ${TARGET}_prebuild POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${DRLPARSER_SCRIPT} ${options}
        VERBATIM
    )
endfunction()
