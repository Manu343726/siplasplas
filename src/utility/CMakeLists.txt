
if(WIN32)
    set(dynamiclibapi)
else()
    set(dynamiclibapi dl)
endif()

add_siplasplas_library(siplasplas-utility
SOURCES
    assert.cpp
    memory_manip.cpp
    printers.cpp
    dynamiclibrary.cpp
    string.cpp
    error_logger.cpp
    logger.cpp
DEPENDS
    ctti-conan
    fmt-conan
    backward-conan
    siplasplas-logger
LINK_LIBS
    ${dynamiclibapi}
)
