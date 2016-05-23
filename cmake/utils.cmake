
# Returns the relative path of the
# current source dir against the source
# tree root.
#
function(sourcetree_relative RESULT)
    file(RELATIVE_PATH relpath "${CMAKE_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}")
    set(${RESULT} "${relpath}" PARENT_SCOPE)
endfunction()

# Gets the headers directory equivalent to the
# current source subtree.
#
#
# The library is organized in two directories:
#
# - Headers: Root at <source-dir>/include/siplasplas/,
#    one directory per module/library target.
#
# - Sources: The source tree, with root at <source-dir>/src/,
#   one subdirectory per module/library. This is where
#   CMakeLists.txt files are.
#
# Given a library target organised in that way, the function returns
# the path to the headers folder of the target/s being processed in this
# sourcetree location..
#
function(headerdir_from_sourcetree RESULT)
    sourcetree_relative(relpath)
    set(includedir "${CMAKE_SOURCE_DIR}/include/siplasplas/${relpath}")
    set(${RESULT}  "${includedir}" PARENT_SCOPE)
endfunction()

function(_copy_dll_dependencies ROOT_TARGET TARGET)
    if((NOT WIN32) OR (NOT (TARGET ${TARGET})))
        return()
    endif()
    get_target_property(target_type ${TARGET} TYPE)
    get_target_property(root_target_type ${ROOT_TARGET} TYPE)
    if((target_type STREQUAL INTERFACE_LIBRARY) OR
       (NOT (root_target_type STREQUAL EXECUTABLE)))
        return()
    endif()
    get_target_property(dependencies ${TARGET} LINK_LIBRARIES)
    if(NOT dependencies)
        return()
    endif()

    message(STATUS " => dlls of target '${TARGET}' for target '${ROOT_TARGET}'")

    set(dest_directory $<TARGET_FILE_DIR:${ROOT_TARGET}>)

    foreach(lib ${dependencies})
        if(TARGET ${lib})
            if(NOT (lib STREQUAL TARGET))
                get_target_property(target_type ${lib} TYPE)
                if(target_type STREQUAL SHARED_LIBRARY)
                    set(dllfile $<TARGET_FILE_DIR:${lib}>/$<TARGET_FILE_NAME:${lib}>)
                endif()
            endif()
        elseif(lib MATCHES ".+\\.dll")
            set(dllfile "${lib}")
        endif()

        if(SIPLASPLAS_VERBOSE_CONFIG)
            set(log COMMAND ${CMAKE_COMMAND} -E echo " - ${dllfile}")
        endif()

        if(dllfile)
            list(APPEND copy_command
                ${log}
                COMMAND ${CMAKE_COMMAND} -E copy
                    \"${dllfile}\"
                    \"${dest_directory}\"
            )
        endif()
    endforeach()

    if(copy_command)
        add_custom_command(TARGET ${ROOT_TARGET} POST_BUILD
            ${copy_command}
            COMMENT "Copying dll dependencies of target ${TARGET}..."
        )
    endif()

    foreach(dep ${dependencies})
       _copy_dll_dependencies(${ROOT_TARGET} "${dep}")
    endforeach()
endfunction()


# Copies dll dependencies of a target to its runtime
# directory.
#
# Recursivelly scans for dll dependencies, so all needed
# dlls are placed within the target output binary (usually
# an executable). This helps to run/debug executables directly
# in the buildtree, without installing them.
#
function(copy_dll_dependencies TARGET)
    _copy_dll_dependencies(${TARGET} ${TARGET})
endfunction()

# Unzips a library list so libraries tagged as
# 'debug', 'optimized', or 'general' are arranged into
# isolated lists.
#
# When passing libraries to commands like target_link_libraries()
# CMake tags them so the linker/user may know which of these libraries
# should be used in several compilation modes.
#
#
# Script took from https://software.lanl.gov/MeshTools/trac/browser/cmake/modules/ParseLibraryList.cmake
function(parse_library_list LIBRARIES)
    foreach( item ${LIBRARIES} )
        if( ${item} MATCHES debug     OR
            ${item} MATCHES optimized OR
            ${item} MATCHES general )

            if( ${item} STREQUAL "debug" )
                set( mylist "_debug_libs" )
            elseif( ${item} STREQUAL "optimized" )
                set( mylist "_opt_libs" )
            elseif( ${item} STREQUAL "general" )
                set( mylist "_gen_libs" )
            endif()
      else()
          list( APPEND ${mylist} ${item} )
      endif()
    endforeach()

    set(DEBUG_LIBS     ${_debug_libs}     PARENT_SCOPE)
    set(OPTIMIZED_LIBS ${_optimized_libs} PARENT_SCOPE)
    set(GENERAL_LIBS   ${_gen_libs}       PARENT_SCOPE)
endfunction()

# Gets the set of target include directories, recusivelly scanning
# dependencies and checking kind of target
function(get_target_include_directories TARGET RESULT)
    if(NOT TARGET ${TARGET})
        set(${RESULT} PARENT_SCOPE)
        return()
    endif()

    get_target_property(type ${TARGET} TYPE)

    if(type STREQUAL "INTERFACE_LIBRARY")
        get_target_property(dependencies ${TARGET} INTERFACE_LINK_LIBRARIES)
        get_target_property(includes     ${TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    else()
        get_target_property(dependencies            "${TARGET}" LINK_LIBRARIES)
        get_target_property(includes                "${TARGET}" INCLUDE_DIRECTORIES)
    endif()

    if(NOT includes)
        set(includes)
    endif()

    if(dependencies)
        set(deps_copy ${dependencies})
        set(dependencies)
        foreach(dep ${deps_copy})
            if(NOT (dep STREQUAL "${TARGET}"))
                list(APPEND dependencies "${dep}")
            endif()
        endforeach()

        foreach(dep ${dependencies})
            get_target_include_directories(${dep} dep_includes)
            list(APPEND includes ${dep_includes})
        endforeach()
    endif()

    if(includes)
        list(REMOVE_DUPLICATES includes)
    endif()
    set(${RESULT} ${includes} PARENT_SCOPE)
endfunction()

