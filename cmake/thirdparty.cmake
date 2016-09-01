include(ExternalProject)

function(libraryfile name type result)
    set(${result} "${CMAKE_${type}_LIBRARY_PREFIX}${name}${CMAKE_${type}_LIBRARY_SUFFIX}" PARENT_SCOPE)
endfunction()

function(generate_external_project NAME NO_CONFIG NO_BUILD EXTERNAL_PROJECT_ARGS)
    if(NO_CONFIG)
        set(configureCommand "CONFIGURE_COMMAND \"\"")
    endif()
    if(NO_BUILD)
        set(buildCommand "BUILD_COMMAND \"\"")
    endif()

    string(REGEX REPLACE "-" "_" outputFile "${NAME}_external_project.cmake")
    string(REGEX REPLACE ";" " " EXTERNAL_PROJECT_ARGS "${EXTERNAL_PROJECT_ARGS}")

    file(WRITE "${CMAKE_BINARY_DIR}/${outputFile}"
"ExternalProject_Add(${NAME}-external-project
    SOURCE_DIR ${CMAKE_BINARY_DIR}/THIRDPARTY/${NAME}/src/${NAME}
    PREFIX ${CMAKE_BINARY_DIR}/THIRDPARTY/${NAME}
    ${EXTERNAL_PROJECT_ARGS}
    ${configureCommand}
    ${buildCommand}
    UPDATE_COMMAND \"\"
    INSTALL_COMMAND \"\"
)")

    include(${CMAKE_BINARY_DIR}/${outputFile})
endfunction()

function(add_siplasplas_thirdparty NAME)
    set(options HEADER_ONLY SKIP_CONFIGURE_STEP SKIP_BUILD_STEP RENAME)
    set(oneValueArgs URL HG_REPOSITORY HG_TAG GIT_REPOSITORY GIT_TAG CONFIGURE_COMMAND BUILD_COMMAND OUTPUT_SOURCE_DIR OUTPUT_BINARY_DIR)
    set(multiValueArgs DEPENDS INCLUDE_DIRS BINARIES COMPILE_OPTIONS COMPILE_DEFINITIONS EXTERNAL_PROJECT_EXTRA_ARGS CMAKE_ARGS CMAKE_EXTRA_ARGS)
    cmake_parse_arguments(THIRDPARTY
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    set(externalProjectArgs)
    set(NO_CONFIG FALSE)
    set(NO_BUILD FALSE)

    if(THIRDPARTY_URL)
        list(APPEND externalProjectArgs URL "${THIRDPARTY_URL}")
    elseif(THIRDPARTY_GIT_REPOSITORY)
        list(APPEND externalProjectArgs GIT_REPOSITORY "${THIRDPARTY_GIT_REPOSITORY}")

        if(THIRDPARTY_GIT_TAG)
            list(APPEND externalProjectArgs GIT_TAG "${THIRDPARTY_GIT_TAG}")
        endif()
    elseif(THIRDPARTY_HG_REPOSITORY)
        list(APPEND externalProjectArgs HG_REPOSITORY "${THIRDPARTY_HG_REPOSITORY}")

        if(THIRDPARTY_HG_TAG)
            list(APPEND externalProjectArgs HG_TAG "${THIRDPARTY_HG_TAGS}")
        endif()
    else()
        message(FATAL_ERROR "No download command for '${NAME}' third party component")
    endif()

    if(NOT THIRDPARTY_HEADER_ONLY)
        if(THIRDPARTY_SKIP_CONFIGURE_STEP)
            set(NO_CONFIG TRUE)
        else()
            if(THIRDPARTY_CONFIGURE_COMMAND)
                list(APPEND externalProjectArgs CONFIGURE_COMMAND ${THIRDPARTY_CONFIGURE_COMMAND})
            elseif(THIRDPARTY_CMAKE_ARGS OR THIRDPARTY_CMAKE_EXTRA_ARGS OR THIRDPARTY_COMPILE_OPTIONS)
                if(THIRDPARTY_CMAKE_ARGS)
                    # Default bypassed args are overrided by user args
                    list(APPEND externalProjectArgs CMAKE_ARGS ${THIRDPARTY_CMAKE_ARGS})
                else()
                    list(APPEND THIRDPARTY_COMPILE_OPTIONS ${CMAKE_CXX_FLAGS})
                    string(REGEX REPLACE " " ";" THIRDPARTY_COMPILE_OPTIONS "${THIRDPARTY_COMPILE_OPTIONS}")
                    # Project ompiler and build settings are bypassed to external
                    # project
                    list(APPEND externalProjectArgs CMAKE_ARGS
                        # Quotes around values are needed, see https://cmake.org/pipermail/cmake/2016-January/062573.html
                        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
                        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
                        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
                        "-DCMAKE_CXX_FLAGS=${THIRDPARTY_COMPILE_OPTIONS}"
                        "${THIRDPARTY_CMAKE_EXTRA_ARGS}"
                    )
                endif()
            endif()
        endif()

        if(THIRDPARTY_SKIP_BUILD_STEP)
            set(NO_BUILD TRUE)
        else()
            if(THIRDPARTY_BUILD_COMMAND)
                list(APPEND externalProjectArgs BUILD_COMMAND ${THIRDPARTY_BUILD_COMMAND})
            else()
                if(THIRDPARTY_CONFIGURE_COMMAND)
                    message(FATAL_ERROR "No build command for '${NAME}' third party with custom configure step")
                endif()
            endif()
        endif()
    else()
        set(NO_CONFIG TRUE)
        set(NO_BUILD TRUE)
    endif()

    list(APPEND externalProjectArgs ${THIRDPARTY_EXTERNAL_PROJECT_EXTRA_ARGS})
    set(external ${NAME}-external-project)

    if(THIRDPARTY_GIT_REPOSITORY)
        set(downloaddir "${CMAKE_BINARY_DIR}/THIRDPARTY/${NAME}/src/${external}")
    else()
        set(downloaddir "${CMAKE_BINARY_DIR}/THIRDPARTY/${NAME}/src/${external}")
    endif()
    set(repodir "${CMAKE_BINARY_DIR}/THIRDPARTY/${NAME}/src/${NAME}")

    generate_external_project(${NAME} ${NO_CONFIG} ${NO_BUILD} "${externalProjectArgs}")

    ExternalProject_Get_Property(${external} source_dir binary_dir)

    if(THIRDPARTY_OUTPUT_SOURCE_DIR)
        set(${THIRDPARTY_OUTPUT_SOURCE_DIR} "${source_dir}" PARENT_SCOPE)
    endif()
    if(THIRDPARTY_OUTPUT_BINARY_DIR)
        set(${THIRDPARTY_OUTPUT_BINARY_DIR} "${binary_dir}" PARENT_SCOPE)
    endif()

    set_target_properties(${external} PROPERTIES EXCLUDE_FROM_ALL TRUE)

    string(REGEX REPLACE "-" "_" FORMATTED_NAME "${NAME}")
    string(TOUPPER "${FORMATTED_NAME}" FORMATTED_NAME)

    set(${FORMATTED_NAME}_SOURCE_DIR "${source_dir}" PARENT_SCOPE)
    set(${FORMATTED_NAME}_BINARY_DIR "${binary_dir}" PARENT_SCOPE)

    add_library(${NAME} INTERFACE)
    add_dependencies(${NAME} ${external})

    set(includedirs)
    foreach(includedir ${THIRDPARTY_INCLUDE_DIRS})
        list(APPEND includedirs
            "${repodir}/${includedir}"
            "${source_dir}/${includedir}"
        )
    endforeach()
    target_include_directories(${NAME} INTERFACE ${includedirs})
    target_link_libraries(${NAME} INTERFACE ${THIRDPARTY_DEPENDS})

    function(print_args var)
        string(REGEX REPLACE "THIRDPARTY_(.+)" "\\1" varname "${var}")
        string(TOLOWER "${varname}" varname)
        string(REGEX REPLACE "_" " " varname "${varname}")

        if(${var})
            message(":: ${varname}:")

            foreach(elem ${${var}})
                message("  - ${elem}")
            endforeach()
        else()
            message(":: No ${varname}")
        endif()
    endfunction()

    message(STATUS "SIPLASPLAS THIRD PARTY LIBRARY ${NAME}")
    if(SIPLASPLAS_VERBOSE_CONFIG)
        foreach(var ${options} ${oneValueArgs} ${multiValueArgs})
            print_args(THIRDPARTY_${var})
        endforeach()
    endif()
endfunction()

function(compute_binary_file NAME TYPE LIBRARY_SUFFIX BINARY_DIR RESULT)
    libraryfile(${NAME} ${TYPE} file)

    get_filename_component(filename "${file}" NAME_WE)
    get_filename_component(fileext  "${file}" EXT)
    set(file "${filename}${LIBRARY_SUFFIX}${fileext}")

    if(MSVC)
        set(file "${CMAKE_BUILD_TYPE}/${file}")
    endif()

    if(SIPLASPLAS_VERBOSE_CONFIG)
        message(STATUS "Component ${NAME} binary file: ${file}")
    endif()

    if(BINARY_DIR)
        set(${RESULT} "${BINARY_DIR}/${file}" PARENT_SCOPE)
    else()
        set(${RESULT} "${file}" PARENT_SCOPE)
    endif()
endfunction()

function(add_siplasplas_thirdparty_component NAME)

    cmake_parse_arguments(COMPONENT
        "DEFAULT;SHARED"
        "THIRD_PARTY;BINARY;BINARY_DIR;BINARY_NAME;LIBRARY_SUFFIX;SHARED_LIBRARY_SUFFIX"
        "INCLUDE_DIRS;DEPENDS;LINK_LIBS"
        ${ARGN}
    )

    if(NOT COMPONENT_THIRD_PARTY)
        message(FATAL_ERROR "No parent third party project given for component '${NAME}'")
    else()
        if(NOT TARGET "${COMPONENT_THIRD_PARTY}")
            message(FATAL_ERROR "Unknown third party library '${COMPONENT_THIRD_PARTY}'")
        else()
            set(external ${COMPONENT_THIRD_PARTY}-external-project)
            set(rename ${COMPONENT_THIRD_PARTY}-rename-sources)
            ExternalProject_Get_Property(${external} source_dir binary_dir)
            set(repodir "${CMAKE_BINARY_DIR}/THIRDPARTY/${COMPONENT_THIRD_PARTY}/src/${COMPONENT_THIRD_PARTY}")
        endif()
    endif()

    if(NOT COMPONENT_BINARY)
        if(NOT COMPONENT_BINARY_NAME)
            set(COMPONENT_BINARY_NAME ${NAME})
        endif()

        if(COMPONENT_SHARED)
            compute_binary_file(${COMPONENT_BINARY_NAME} SHARED "${COMPONENT_LIBRARY_SUFFIX}${COMPONENT_SHARED_LIBRARY_SUFFIX}" "${COMPONENT_BINARY_DIR}" libfile)
            install(FILES "${binary_dir}/${libfile}" DESTINATION lib)

            # We should import the location of the import lib (.lib) too to link with the dll
            # (In dll platforms you link against the import lib, not the dynamic library)
            if(MSVC)
                compute_binary_file(${COMPONENT_BINARY_NAME} IMPORT "${COMPONENT_LIBRARY_SUFFIX}" "${COMPONENT_BINARY_DIR}" importedlibfile)
                install(FILES "${binary_dir}/${importedlibfile}" DESTINATION lib)
            endif()

        else()
            compute_binary_file(${COMPONENT_BINARY_NAME} STATIC "${COMPONENT_LIBRARY_SUFFIX}" "${COMPONENT_BINARY_DIR}" libfile)
        endif()

        set(COMPONENT_BINARY "${libfile}")
    endif()

    message(STATUS "SIPLASPLAS THIRD PARTY COMPONENT '${NAME}' FROM '${COMPONENT_THIRD_PARTY}'")

    set(importedlib ${COMPONENT_THIRD_PARTY}-${NAME}-imported-lib)
    set(interfacelib ${COMPONENT_THIRD_PARTY}-${NAME})

    if(COMPONENT_SHARED)
        set(libtype SHARED)
    else()
        set(libtype STATIC)
    endif()

    add_library(${importedlib} IMPORTED GLOBAL ${libtype})

    foreach(includedir ${COMPONENT_INCLUDE_DIRS})
        list(APPEND includedirs
            "${repodir}/${includedir}"
            "${source_dir}/${includedir}"
        )
    endforeach()

    set_target_properties(${importedlib} PROPERTIES
        IMPORTED_LOCATION "${binary_dir}/${COMPONENT_BINARY}"
    )

    if(importedlibfile)
        if(SIPLASPLAS_VERBOSE_CONFIG)
            message(STATUS "Adding implementation lib (${importedlibfile}) for component ${NAME} of thirdparty ${COMPONENT_THIRD_PARTY}")
        endif()

        set_target_properties(${importedlib} PROPERTIES
            IMPORTED_IMPLIB "${binary_dir}/${importedlibfile}"
        )
    endif()

    add_library(${interfacelib} INTERFACE)
    target_include_directories(${interfacelib} INTERFACE ${includedirs})
    target_link_libraries(${interfacelib} INTERFACE ${importedlib})

    foreach(dependency ${COMPONENT_DEPENDS})
        target_link_libraries(${interfacelib} INTERFACE ${COMPONENT_THIRD_PARTY}-${dependency})
    endforeach()
    foreach(lib ${COMPONENT_LINK_LIBS})
        target_link_libraries(${interfacelib} INTERFACE ${lib})
    endforeach()

    if(COMPONENT_DEFAULT)
        target_link_libraries(${COMPONENT_THIRD_PARTY} INTERFACE ${interfacelib})
    else()
        target_link_libraries(${interfacelib} INTERFACE ${COMPONENT_THIRD_PARTY})
    endif()
endfunction()

