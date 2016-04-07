
function(add_test NAME)
    add_siplasplas_test(${NAME}
        ${ARGN}
        SOURCES ${NAME}.cpp
        INCLUDE_DIRS "${CMAKE_SOURCE_DIR/tests}"
        NAMESPACE tests
    )
endfunction()

add_test(variant          DEPENDS siplasplas-variant)
add_test(linear_allocator DEPENDS siplasplas-allocator)
