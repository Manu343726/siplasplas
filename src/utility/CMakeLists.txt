
add_siplasplas_library(siplasplas-utility
SOURCES
    memory_manip.cpp
    printers.cpp
    dynamiclibrary.cpp
    string.cpp
DEPENDS
    ctti
    fmt
LINK_LIBS
    dl
)
