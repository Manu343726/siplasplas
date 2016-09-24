if(NOT CHAISCRIPT_VERSION)
    set(CHAISCRIPT_VERSION 5.8.1)
endif()

add_siplasplas_thirdparty(chaiscript HEADER_ONLY
URL
    "https://github.com/ChaiScript/ChaiScript/archive/v${CHAISCRIPT_VERSION}.zip"
HEADER_DIRS
    include/chaiscript
)

find_package(Threads REQUIRED)
target_link_libraries(chaiscript INTERFACE ${CMAKE_THREAD_LIBS_INIT})
