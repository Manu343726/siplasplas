add_subdirectory(backward-cpp)
add_subdirectory(boost)
add_subdirectory(efsw)
add_subdirectory(libexecstream)
if(NOT SIPLASPLAS_CI_BUILD)
    # Cannot configure SFML with current
    # Travis CI setup. See https://github.com/GueimUCM/siplasplas/issues/35
    add_subdirectory(imgui-sfml)
else()
    message(WARNING "Imgui-SFML disabled (See https://github.com/GueimUCM/siplasplas/issues/35)")
endif()
