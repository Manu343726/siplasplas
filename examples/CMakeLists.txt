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


add_subdirectory(backward-cpp)
add_subdirectory(boost)
add_subdirectory(utility)
add_subdirectory(allocator)
add_subdirectory(reflection)
add_subdirectory(variant)
add_subdirectory(metatype)
