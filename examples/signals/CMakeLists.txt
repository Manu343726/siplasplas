find_package(Threads REQUIRED)

if(SIPLASPLAS_CI_BUILD)
    set(EXCLUDE_FROM_RUN_ALL EXCLUDE_FROM_RUN_ALL)
endif()

add_siplasplas_example_simple(signals ${EXCLUDE_FROM_RUN_ALL} DEPENDS siplasplas-signals ${CMAKE_THREAD_LIBS_INIT})
configure_siplasplas_reflection(examples-signals)

set(PRODUCER_MILLISECONDS 3000)
set(CONSUMER_MILLISECONDS 1000)

target_compile_definitions(examples-signals PRIVATE
    -DSIPLASPLAS_EXAMPLES_SIGNALS_PRODUCER_MS=${PRODUCER_MILLISECONDS}
    -DSIPLASPLAS_EXAMPLES_SIGNALS_CONSUMER_MS=${CONSUMER_MILLISECONDS}
)

