execute_process(
    COMMAND conan install ${CMAKE_SOURCE_DIR}/3rdParty --build=outdated
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
conan_define_targets()
include(${CONAN_CMAKE-UTILS_ROOT}/conan.cmake)
