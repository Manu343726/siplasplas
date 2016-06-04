find_package(Threads REQUIRED)

add_reflection_example(signals DEPENDS siplasplas-signals ${CMAKE_THREAD_LIBS_INIT})

