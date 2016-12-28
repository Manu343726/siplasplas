include(thirdparty)
include(pip)

add_conan_library(ctti)
add_conan_library(nljson)
add_conan_library(fmt)
add_conan_library(spdlog DEPENDS fmt-conan)
add_conan_library(backward)
add_conan_library(googletest)
add_conan_library(googlemock DEPENDS googletest-conan EXTRA_LIBS gmock_main)
add_subdirectory(standardese)
add_conan_library(efsw)
add_conan_library(readerwriterqueue)
add_conan_library(libexecstream)
add_conan_library(clang)

if(SIPLASPLAS_BUILD_EXAMPLES)
    add_conan_library(chaiscript)
    add_subdirectory(protobuf)

    if(NOT SIPLASPLAS_CI_BUILD)
        # Cannot configure SFML with current
        # Travis CI setup. See https://github.com/GueimUCM/siplasplas/issues/35
        add_subdirectory(imgui-sfml) # no conan
    else()
        message(WARNING "Imgui-SFML disabled (See https://github.com/GueimUCM/siplasplas/issues/35)")
    endif()
endif()
