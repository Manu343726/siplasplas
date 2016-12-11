add_siplasplas_library(siplasplas-cmake
SOURCES
    project.cpp
    target.cpp
    logger.cpp
DEPENDS
    libexecstream-conan
    nljson-conan
    siplasplas-fswatch
)

configure_siplasplas_reflection(siplasplas-cmake)
