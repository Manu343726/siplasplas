include(${CMAKE_SOURCE_DIR}/src/reflection/parser/drlparser.cmake)

function(add_example NAME)
    cmake_parse_arguments(
        "EXAMPLE"
        ""
        ""
        "INSTALL_SOURCES"
        ${ARGN}
    )

    add_siplasplas_executable(${NAME}
        ${EXAMPLE_UNPARSED_ARGUMENTS}
        SOURCES ${NAME}.cpp
        INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/examples"
        NAMESPACE examples
    )

    target_include_directories(examples-${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    install_siplasplas_example(examples-${NAME}
        SOURCES ${EXAMPLE_INSTALL_SOURCES} ${NAME}.cpp
    )
endfunction()

function(add_reflection_example NAME)
    cmake_parse_arguments(ARGS
        ""
        ""
        "DEPENDS;INSTALL_SOURCES"
        ${ARGN}
    )

    list(APPEND ARGS_DEPENDS siplasplas-reflection)

    if(ARGS_INSTALL_SOURCES)
        set(install_sources INSTALL_SOURCES ${ARGS_INSTALL_SOURCES})
    endif()

    add_example(${NAME} DEPENDS ${ARGS_DEPENDS} ${install_sources} ${ARGS_UNPARSED_ARGUMENTS})
    reflection_target(examples-${NAME})
endfunction()

add_subdirectory(backward-cpp)
add_subdirectory(boost)
add_subdirectory(utility)
add_subdirectory(allocator)
add_subdirectory(reflection)
add_subdirectory(variant)
add_subdirectory(metatype)
add_subdirectory(signals)
add_subdirectory(efsw)
add_subdirectory(imgui-sfml)
