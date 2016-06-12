add_siplasplas_library(siplasplas-fswatch
SOURCES
    fslistener.cpp
DEPENDS
    efsw
    siplasplas-signals
)

configure_siplasplas_reflection(siplasplas-fswatch)
