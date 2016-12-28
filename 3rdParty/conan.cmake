execute_process(
    COMMAND conan install --build=outdated --file "${CMAKE_SOURCE_DIR}/3rdParty/conanfile.txt"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include(${CMAKE_SOURCE_DIR}/3rdParty/conanbuildinfo.cmake)
conan_basic_setup()
conan_define_targets()
include(${CONAN_CMAKE-UTILS_ROOT}/conan.cmake)
