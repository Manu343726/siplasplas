function(clangxx_executable _ret)
    if(SIPLASPLAS_CLANGXX)
        set(${_ret} "${SIPLASPLAS_CLANGXX}" PARENT_SCOPE)
    elseif(CMAKE_CXX_COMPILER_ID  MATCHES "Clang")
        set(${_ret} "${CMAKE_CXX_COMPILER}" PARENT_SCOPE)
    else()
        if(NOT clangxx)
            find_program(clangxx clang++)

            if(clangxx)
                message(STATUS "Found clang++ program: ${clangxx}")
                set(${_ret} "${clangxx}" PARENT_SCOPE)
            else()
                message(WARNING "clang++ not found")
                set(${_ret} PARENT_SCOPE)
            endif()
        else()
            set(${_ret} "${clangxx}" PARENT_SCOPE)
        endif()
    endif()
endfunction()

# Adapted from https://github.com/biicode/boost
function(clangxx_version _ret)
    clangxx_executable(clangxx)

    if(WIN32)
        # All this stuff around execute_process() is to handle Windows paths with spaces
        # such as "C:\Program Files (x86)\foo\"
        # I love Windows
        get_filename_component(dir "${clangxx}" DIRECTORY)
        get_filename_component(name "${clangxx}" NAME)

        execute_process(
            COMMAND ${name} --version
            OUTPUT_VARIABLE stdout
            ERROR_VARIABLE stderr
            RESULT_VARIABLE result_code
            WORKING_DIRECTORY "${dir}"
        )
    else()
        execute_process(
            COMMAND ${clangxx} --version
            OUTPUT_VARIABLE stdout
            ERROR_VARIABLE stderr
            RESULT_VARIABLE result_code
            WORKING_DIRECTORY "${dir}"
        )
    endif()

    string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+\\.[0-9]+).*" "\\1" version_string "${stdout}")

    if(SIPLASPLAS_VERBOSE_CONFIG)
        message(STATUS "clang executable: ${clangxx}")
        message(STATUS "clang version output: ${stdout}")
        message(STATUS "clang version error: ${stderr}")
        message(STATUS "clang version result code: ${result_code}")
        message(STATUS "clang version parsed: ${version_string}")
    endif()

    set(${_ret} ${version_string} PARENT_SCOPE)
endfunction()

if(NOT SIPLASPLAS_LIBCLANG_VERSION)
    # Set libclang version to the installed clang version
    clangxx_version(SIPLASPLAS_LIBCLANG_VERSION)
endif()

if(SIPLASPLAS_DOWNLOAD_LIBCLANG)
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
        if(NOT SIPLASPLAS_CI_BUILD)
            set(progress SHOW_PROGRESS)
        endif()

        file(DOWNLOAD "${url}" "${package_path}" ${progress})
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
            string(STRIP llvm_root "${llvm_root}")
        else()
            message(FATAL_ERROR "Extracted llvm directory \"${llvm_contents}\" is not a directory")
        endif()
    else()
        foreach(file ${llvm_contents})
            message(STATUS "  (llvm content found) ${CMAKE_BINARY_DIR}/llvm/${file}")
        endforeach()
        message(FATAL_ERROR "LLVM package contents not recognized, only a root directory was expected")
    endif()

    if(WIN32)
        set(libclang_library "${llvm_root}/lib/libclang.dll")
    else()
        if(APPLE)
            set(libclang_library "${llvm_root}/lib/libclang.dylib")
        else()
            set(libclang_library "${llvm_root}/lib/libclang.so")
        endif()
    endif()

    # Write output variables:
    if(libclang_library AND EXISTS "${libclang_library}")
        set(SIPLASPLAS_LIBCLANG_LIBRARY "${libclang_library}")
    else()
        message(STATUS "Could not find libclang library at ${libclang_library}")
        message(FATAL_ERROR "Could not find libclang library at ${llvm_root}/lib")
    endif()
    set(SIPLASPLAS_LIBCLANG_INCLUDE_DIR "${llvm_root}/include")

    # Use downloaded clang includes only if the compiler
    # is not a clang, else compilation may fail.
    # Later libclang_system_include_dir() function guesses
    # the right path from the clang compiler config
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(STATUS "libclang ${SIPLASPLAS_LIBCLANG_VERSION} downloaded, but compiler is ${CMAKE_CXX_COMPILER_ID}")
        unset(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR)
    else()
        message(STATUS "Using downloaded libclang ${SIPLASPLAS_LIBCLANG_VERSION} system includes")
        set(SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR "${llvm_root}/lib/clang/${SIPLASPLAS_LIBCLANG_VERSION}/include")
    endif()
else()
    if(NOT SIPLASPLAS_LIBCLANG_LIBRARY)
        if(WIN32)
            clangxx_executable(clangxx)
            get_filename_component(llvm_bin "${clangxx}" DIRECTORY)
            message(STATUS "LLVM/bin/: ${llvm_bin}")
            find_library(libclang_library
                NAMES clang libclang.dll
                HINTS "${llvm_bin}"
            )
        else()
            find_library(libclang_library clang libclang)
        endif()

        if(NOT libclang_library AND EXISTS "${llvm_bin}/libclang.dll")
            message(STATUS "${llvm_bin}/libclang.dll found by cmake FS IO")
            set(libclang_library "${llvm_bin}/libclang.dll")
        endif()

        if(libclang_library)
            set(SIPLASPLAS_LIBCLANG_LIBRARY "${libclang_library}")
        else()
            message(FATAL_ERROR "libclang library not found")
        endif()
    endif()
endif()

string(REGEX REPLACE "([0-9]+\\.[0-9]+)\\.[0-9]+" "\\1" SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR "${SIPLASPLAS_LIBCLANG_VERSION}")

if(SIPLASPLAS_VERBOSE_CONFIG)
    message(STATUS "libclang version: ${SIPLASPLAS_LIBCLANG_VERSION} (${SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR})")
    message(STATUS "libclang library: ${SIPLASPLAS_LIBCLANG_LIBRARY}")
    message(STATUS "libclang include dir: ${SIPLASPLAS_LIBCLANG_INCLUDE_DIR}")
    message(STATUS "libclang system include dir: ${SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR}")
endif()

function(gxx_executable _ret)
    if(SIPLASPLAS_GXX)
        set(${_ret} "${SIPLASPLAS_GXX}" PARENT_SCOPE)
    else()
        if(CMAKE_COMPILER_IS_GNUCXX)
            set(${_ret} "${CMAKE_CXX_COMPILER}" PARENT_SCOPE)
        else()
            if(NOT gxx)
                find_program(gxx g++)

                if(gxx)
                    message(STATUS "Found g++ program: ${gxx}")
                    set(${_ret} "${gxx}" PARENT_SCOPE)
                else()
                    message(WARNING "g++ not found")
                    set(${_ret} PARENT_SCOPE)
                endif()
            else()
                set(${_ret} "${gxx}" PARENT_SCOPE)
            endif()
        endif()
    endif()
endfunction()

function(gxx_version _ret)
    gxx_executable(gxx)

    execute_process( COMMAND ${gxx} -dumpversion OUTPUT_VARIABLE version_string)
    string (REGEX REPLACE "([0-9])\\.([0-9])\\.([0-9])" "\\1.\\2.\\3" version_string ${version_string})
    string(STRIP ${version_string} version_string) #Remove extra newline character

    set(${_ret} "${version_string}" PARENT_SCOPE)
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
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        clangxx_executable(clangxx)
        list(APPEND compilers clangxx)
    elseif(CMAKE_COMPILER_IS_GNUCXX)
        gxx_executable(gxx)
        list(APPEND compilers gxx)
    elseif(MSVC)
        clangxx_executable(clangxx)
        list(APPEND compilers clangxx)
    else()
        message(FATA_ERROR "Unsupported compiler")
    endif()

    foreach(compiler_exec ${compilers})
        # We want DRLParser to work even if MinGW is not available on Windows,
        # but note that would add some innaccuracies when parsing with libclang
        # (Missing standard library headers, etc)
        if(NOT ${compiler_exec})
            continue()
        endif()

        if(SIPLASPLAS_VERBOSE_CONFIG)
            message(STATUS "Asking for ${${compiler_exec}} (${compiler_exec}) include dirs...")
        endif()
        if(compiler_exec MATCHES "clangxx")
            set(stdlib_option "-stdlib=${stdlib}")
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E echo ""
            COMMAND ${${compiler_exec}} -std=c++11 ${stdlib_option} -v -x c++ -E -
            OUTPUT_VARIABLE out
            ERROR_VARIABLE err
        )

        if(SIPLASPLAS_VERBOSE_CONFIG)
            message(STATUS "output from ${${compiler_exec}}: ${out}")
            message(STATUS "error output from ${${compiler_exec}}: ${err}")
        endif()

        # Since GNU has localization in most of their tools (GCC included)
        # the diagnostics are translated to the different user languages.
        # We have to handle the different languages...
        #
        # The fun thing of this is that GNU translation to Spanish
        # seems to be done by a bad Google Translate bot or something

        set(locale_component_separator "__siplasplas_locale_component_separator__")

        set(include_search_locales
            "english
${locale_component_separator}
#include \"...\" search starts here:
#include <...> search starts here:
${locale_component_separator}
End of search list."

            "spanish
${locale_component_separator}
la búsqueda de #include \"...\" inicia aquí:
la búsqueda de #include <...> inicia aquí:
${locale_component_separator}
Fin de la lista de búsqueda"
        )

        foreach(locale ${include_search_locales})
            string(REGEX REPLACE "(.*)\n${locale_component_separator}\n.*\n${locale_component_separator}\n.*" "\\1" language "${locale}")
            string(REGEX REPLACE ".*\n${locale_component_separator}\n(.*)\n${locale_component_separator}\n.*" "\\1" header "${locale}")
            string(REGEX REPLACE ".*\n${locale_component_separator}\n.*\n${locale_component_separator}\n(.*)" "\\1" footer "${locale}")

            if(SIPLASPLAS_VERBOSE_CONFIG)
                message(STATUS "Checking ${${compiler_exec}} output with ${language} language...")
                message(STATUS " - header: ${header}")
                message(STATUS " - footer: ${footer}")
            endif()

            string(REGEX MATCH "
${header}
(.+)
${footer}"
                    includes "${err}\n STDOUT: ${out}")

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
        endforeach()

        if(includedirs)
            if(WIN32)
                # Explicitly add MinGW libstdc++ include dir
                gxx_version(gxx_version)
                list(APPEND includedirs
                    "C:/MinGW/include/c++/${gxx_version}"
                    "C:/MinGW/include/c++/${gxx_version}/x86_64-w64-mingw32"
                )
            endif()

            list(REMOVE_DUPLICATES includedirs)
            set(${INCLUDES} "${includedirs}" PARENT_SCOPE)
            return()
        endif()
    endforeach()

    message(FATAL_ERROR "No ${stdlib} include directories found")
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

set(LIBCLANG_EXTRA_COMPILE_OPTIONS -std=c++14 -stdlib=libstdc++)
