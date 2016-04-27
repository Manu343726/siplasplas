include(ExternalProject)

if(NOT CHAISCRIPT_VERSION)
    set(CHAISCRIPT_VERSION 5.8.1)
endif()

ExternalProject_Add(chaiscript-external
    URL "https://github.com/ChaiScript/ChaiScript/archive/v${CHAISCRIPT_VERSION}.zip"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/chaiscript
)

ExternalProject_Get_Property(chaiscript-external source_dir binary_dir)

find_package(Threads REQUIRED)

add_siplasplas_header_only_library(chaiscript
INCLUDE_DIRS
    ${source_dir}/include
LINK_LIBS
    dl
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(chaiscript chaiscript-external)

