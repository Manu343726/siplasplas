
find_package(Threads)

add_siplasplas_library(siplasplas-jobs
SOURCES
    job.cpp
    pool.cpp
    worker.cpp
    engine.cpp
    logger.cpp
    jobqueue.cpp
DEPENDS
    siplasplas-utility
    CONAN_PKG::concurrentqueue
    ${CMAKE_THREAD_LIBS_INIT}
)
