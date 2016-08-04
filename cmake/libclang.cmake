if(SIPLASPLAS_DOWNLOAD_LIBCLANG)
    if(NOT SIPLASPLAS_LIBCLANG_VERSION)
        set(SIPLASPLAS_LIBCLANG_VERSION 3.8.0)
    endif()

    if(NOT SIPLASPLAS_LIBCLANG_PACKAGE)
        # Get the name of the llvm package for this platform:

        if(CMAKE_SYSTEM_NAME MATCHES "Linux")
            # We're using the ubuntu package for all linux distros.
            # This may fail in ArchLinux due to missing curses library.
            # See https://github.com/Valloric/YouCompleteMe/issues/778
            #
            # TL;DR: Install ncurses5-compat-libs package from AUR

            set(platform "linux-gnu-ubuntu-14.04")
            set(ext ".tar.xz")
        elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
            set(platform "apple-darwin")
            set(ext ".tar.xz")
        else()
            message(FATAL_ERROR "Platform not supported. Cannot infer libclang package")
        endif()

        set(SIPLASPLAS_LIBCLANG_PACKAGE "clang+llvm-${SIPLASPLAS_LIBCLANG_VERSION}-x86_64-${platform}${ext}")
    endif()

    set(package_path "${CMAKE_BINARY_DIR}/llvmbundle")
    set(url "http://llvm.org/releases/${SIPLASPLAS_LIBCLANG_VERSION}/${SIPLASPLAS_LIBCLANG_PACKAGE}")

    if(NOT EXISTS "${package_path}")
        message(STATUS "Downloading libclang ${SIPLASPLAS_LIBCLANG_VERSION} from \"${url}\"...")
        file(DOWNLOAD "${url}" "${package_path}" SHOW_PROGRESS)
    endif()

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/llvm")
        file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/llvm/")

        message(STATUS "Extracting llvm...")

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xvf ${package_path}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/llvm
        )
    endif()

    # This is the most trickie part: Since get_filename_component(NAME_WE) gets
    # the wrong name (the name is split at the first dot, i.e. "clang+llvm-3.8-foo,tar.gz"
    # becomes "clang+llvm-3") we instead extract llvm in a known llvm/ directory and then
    # glob its contents, which should be a unique "clang+llvm..." directory:

    file(GLOB llvm_contents RELATIVE "${CMAKE_BINARY_DIR}/llvm" "${CMAKE_BINARY_DIR}/llvm/*")
    list(LENGTH llvm_contents length)

    if(length EQUAL 1)
        if(IS_DIRECTORY "${CMAKE_BINARY_DIR}/llvm/${llvm_contents}")
            set(llvm_root "${CMAKE_BINARY_DIR}/llvm/${llvm_contents}")
        else()
            message(FATAL_ERROR "Extracted llvm directory \"${llvm_contents}\" is not a directory")
        endif()
    else()
        foreach(file ${llvm_contents})
            message(STATUS "  (llvm content found) ${CMAKE_BINARY_DIR}/llvm/${file}")
        endforeach()
        message(FATAL_ERROR "LLVM package contents not recognized, only a root directory was expected")
    endif()

    find_library(libclang_library NAMES clang libclang HINTS "${llvm_root}/lib")

    # Write output variables:
    if(libclang_library)
        set(SIPLASPLAS_LIBCLANG_LIBRARY "${libclang_library}")
    else()
        message(FATAL_ERROR "Could not find libclang library at ${llvm_root}/lib")
    endif()
    set(SIPLASPLAS_LIBCLANG_INCLUDE_DIR "${llvm_root}/include")
    set(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR "${llvm_root}/lib/clang/${SIPLASPLAS_LIBCLANG_VERSION}/include")
endif()

if(SIPLASPLAS_INSTALL_PYTHON_LIBCLANG)
    find_program(pip NAMES pip2 pip)

    if(pip)
        execute_process(COMMAND ${pip} --version OUTPUT_VARIABLE pip_version)
        string(REGEX REPLACE ".*\\(python ([0-9]+)\\.[0-9]+.*\\)" "\\1" pip_python_version_major "${pip_version}")
        string(REGEX REPLACE ".*\\(python [0-9]+\\.([0-9])+.*\\)" "\\1" pip_python_version_minor "${pip_version}")
        string(STRIP "${pip_python_version_major}" pip_python_version_major)
        string(STRIP "${pip_python_version_minor}" pip_python_version_minor)
        set(PIP_PYTHON_VERSION "${pip_python_version_major}.${pip_python_version_minor}")

        if(PIP_PYTHON_VERSION VERSION_EQUAL SIPLASPLAS_DRLPARSER_PYTHON_VERSION)
            message(STATUS "pip found: ${pip}, python ${pip_python_version_major}.${pip_python_version_minor}")
        else()
            message(FATAL_ERROR "Found pip ${pip} version (${PIP_PYTHON_VERSION}) does not match python version required by DRLParser (${SIPLASPLAS_DRLPARSER_PYTHON_VERSION})")
        endif()
    else()
        message(FATAL_ERROR "pip not found")
    endif()
endif()

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
        set(dir "${SIPLASPLAS_LIBCLANG_INCLUDE_DIR}")
    else()
        find_path(libclang_include_dir clang-c/Index.h)

        if(libclang_include_dir)
            set(dir "${libclang_include_dir}")
        else()
            message(FATAL_ERROR "Cannot find libclang include dir (Location of clang-c/Index.h). Make sure libclang is installed")
        endif()
    endif()

    set(${_ret} "${dir}" PARENT_SCOPE)
    set(clangc "${dir}/clang-c")

    if(SIPLASPLAS_CHECK_LIBCLANG_HEADERS)
        set(headers
            BuildSystem.h
            CXCompilationDatabase.h
            CXErrorCode.h
            CXString.h
            Documentation.h
            Index.h
            Platform.h
        )

        message(STATUS "Checking libclang headers in ${clangc}:")

        foreach(header ${headers})
            set(fullpath "${clangc}/${header}")

            if(EXISTS "${fullpath}")
                message(STATUS " - ${header} found")
            else()
                message(STATUS " - ${header} NOT FOUND!")
                set(error TRUE)
            endif()
        endforeach()

        if(error)
            execute_process(COMMAND ls -l "${dir}" OUTPUT_VARIABLE ls)
            message(STATUS "Contents of ${dir}:")
            message("${ls}")
            execute_process(COMMAND ls -l "${clangc}" OUTPUT_VARIABLE ls)
            message(STATUS "Contents of ${clangc}:")
            message("${ls}")
            message(FATAL_ERROR "One or more libclang headers not found")
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

function(libclang_library _ret)
    if(SIPLASPLAS_LIBCLANG_LIBRARY)
        set(${_ret} "${SIPLASPLAS_LIBCLANG_LIBRARY}" PARENT_SCOPE)
    else()
        find_package(LibClang REQUIRED)

        set(${_ret} "${LIBCLANG_LIBRARIES}" PARENT_SCOPE)
    endif()
endfunction()
