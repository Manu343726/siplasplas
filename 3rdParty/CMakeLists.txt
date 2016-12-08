include(thirdparty)
include(pip)

execute_process(
    COMMAND conan install --build=outdated
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include(conanbuildinfo.cmake)
include(conan.cmake)
conan_basic_setup()

add_conan_library(ctti)
add_conan_library(nljson)
add_conan_library(fmt)
add_conan_library(spdlog DEPENDS fmt-conan)
add_subdirectory(backward-cpp)   # no conan
add_conan_library(googletest)
add_conan_library(googlemock DEPENDS googletest-conan EXTRA_LIBS gmock_main) # conan
add_subdirectory(standardese)    # no conan
add_subdirectory(efsw)           # no conan
add_subdirectory(readerwriterqueue) # no conan
add_subdirectory(libexecstream)     # no conan

if(SIPLASPLAS_BUILD_EXAMPLES)
    add_subdirectory(chaiscript) # no conan
    add_subdirectory(protobuf) # conan

    if(NOT SIPLASPLAS_CI_BUILD)
        # Cannot configure SFML with current
        # Travis CI setup. See https://github.com/GueimUCM/siplasplas/issues/35
        add_subdirectory(imgui-sfml) # no conan
    else()
        message(WARNING "Imgui-SFML disabled (See https://github.com/GueimUCM/siplasplas/issues/35)")
    endif()
endif()
