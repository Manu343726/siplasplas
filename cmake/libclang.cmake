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

function(clangxx_stdlib_includes stdlib INCLUDES)
    clangxx_executable(clangxx)

    execute_process(
        COMMAND ${CMAKE_COMMAND} -E echo ""
        COMMAND ${clangxx} -std=c++11 -stdlib=${stdlib} -v -x c++ -E -
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
        string(STRIP "${includedir}" includedir)

        list(APPEND includedirs ${includedir})
    endforeach()

    if(NOT includedirs)
        message(FATAL_ERROR "No ${stdlib} include directory found")
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

function(libclang_include_dir _ret)
    if(SIPLASPLAS_LIBCLANG_INCLUDE_DIR)
        set(${_ret} "${SIPLASPLAS_LIBCLANG_INCLUDE_DIR}" PARENT_SCOPE)
    else()
        find_path(libclang_include_dir clang-c/Index.h)

        if(libclang_include_dir)
            set(${_ret} "${libclang_include_dir}" PARENT_SCOPE)
        else()
            message(FATAL_ERROR "Cannot find libclang include dir (Location of clang-c/Index.h). Make sure libclang is installed")
        endif()
    endif()
endfunction()

# Needed to solve this issue http://clang.llvm.org/docs/LibTooling.html#builtin-includes
function(libclang_system_include_dir _ret)
    if(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
        set(${_ret} "${SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR}" PARENT_SCOPE)
    else()
        clangxx_version(CLANG_VERSION)
        clangxx_executable(CLANG_EXEC)

        if(WIN32)
            windows_path("${CLANG_EXEC}" CLANG_EXEC)
        endif()

        get_filename_component(CLANG_PATH "${CLANG_EXEC}" DIRECTORY)
        get_filename_component(CLANG_PATH "${CLANG_PATH}" REALPATH)

        set(${_ret} "${CLANG_PATH}/../lib/clang/${CLANG_VERSION}" PARENT_SCOPE)
    endif()
endfunction()

