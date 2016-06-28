include(thirdparty)

add_subdirectory(ctti)
add_subdirectory(chaiscript)
add_subdirectory(json4moderncpp)
add_subdirectory(spdlog)
add_subdirectory(backward-cpp)
add_subdirectory(fmt)
add_subdirectory(googletest)
add_subdirectory(standardese)
add_subdirectory(efsw)
add_subdirectory(readerwriterqueue)
add_subdirectory(libexecstream)

if(NOT SIPLASPLAS_CI_BUILD)
    # Cannot configure SFML with current
    # Travis CI setup. See https://github.com/GueimUCM/siplasplas/issues/35
    add_subdirectory(imgui-sfml)
else()
    message(WARNING "Imgui-SFML disabled (See https://github.com/GueimUCM/siplasplas/issues/35)")
endif()
