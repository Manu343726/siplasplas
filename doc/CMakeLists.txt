# Adapted from Louis Dionne's Boost.Hana config

find_package(Doxygen REQUIRED)

configure_file(Doxyfile.in Doxyfile)

add_custom_target(doc
    COMMAND ${CMAKE_COMMAND} -E remove_directory html
    COMMAND ! ${DOXYGEN_EXECUTABLE} Doxyfile 2>&1 | grep -E "warning|error"
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)
