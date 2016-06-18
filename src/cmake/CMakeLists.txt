add_siplasplas_library(siplasplas-cmake
SOURCES
    project.cpp
    logger.cpp
DEPENDS
    libexecstream
    siplasplas-fswatch
)

configure_siplasplas_reflection(siplasplas-cmake)
