add_siplasplas_thirdparty(sfml
GIT_REPOSITORY
    "https://github.com/SFML/SFML"
CMAKE_EXTRA_ARGS
    -DSFML_BUILD_DOCS=OFF
    -DSFML_BUILD_EXAMPLES=OFF
INCLUDE_DIRS
    include/
)

set(sfml-components
    sfml-system
    sfml-window
    sfml-network
    sfml-graphics
    sfml-audio
)

if(WIN32)
    list(APPEND sfml-components sfml-main)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(suffix -d)
endif()

foreach(component ${sfml-components})
    add_siplasplas_thirdparty_component(${component} SHARED
    THIRD_PARTY
        sfml
    LIBRARY_SUFFIX
        ${suffix}
    BINARY_DIR
        lib
    )
    list(APPEND sfml-targets sfml-${component})
endforeach()

add_siplasplas_library(imgui-sfml
SOURCES
    imgui-sfml/imgui-SFML.cpp
    imgui-sfml/imgui.cpp
    imgui-sfml/imgui_draw.cpp
    imgui-sfml/imgui_demo.cpp
INCLUDE_DIRS
    imgui-sfml
    "${CMAKE_CURRENT_SOURCE_DIR}"
DEPENDS
    ${sfml-targets}
)
