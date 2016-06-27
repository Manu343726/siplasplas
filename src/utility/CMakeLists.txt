
add_siplasplas_library(siplasplas-utility
SOURCES
    assert.cpp
    memory_manip.cpp
    printers.cpp
    dynamiclibrary.cpp
    string.cpp
    error_logger.cpp
DEPENDS
    ctti
    fmt
    backward-cpp
    siplasplas-logger
LINK_LIBS
    dl
)
