function(add_test NAME)
    add_siplasplas_test(${NAME}
        ${ARGN}
        SOURCES ${NAME}.cpp
        INCLUDE_DIRS "${CMAKE_SOURCE_DIR/tests}"
        NAMESPACE tests
    )
endfunction()

message(STATUS "Configuring tests...")

add_subdirectory(allocator)
add_subdirectory(variant)
add_subdirectory(reflection)
