if(SIPLASPLAS_LIBRARIES_STATIC)
    set(type STATIC)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(BUILD_SFML_STATIC -DBUILD_SHARED_LIBS=FALSE)
else()
    set(type SHARED)
endif()

set(sfml-components
    sfml-system
    sfml-network
    sfml-window
    sfml-graphics
    sfml-audio
)

if(SIPLASPLAS_USE_SYSTEM_SFML)
    find_package(SFML 2 COMPONENTS ${sfml-components} REQUIRED)
    set(sfml-targets ${SFML_LIBRARIES})
else()
    add_siplasplas_thirdparty(sfml
    GIT_REPOSITORY
        "https://github.com/SFML/SFML"
    CMAKE_EXTRA_ARGS
        -DSFML_BUILD_EXAMPLES=OFF
        ${BUILD_SFML_STATIC}
    INCLUDE_DIRS
        include/
    )

    if(WIN32)
        list(APPEND sfml-components sfml-main)
    endif()

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(suffix -d)
    endif()

    foreach(component ${sfml-components})
        add_siplasplas_thirdparty_component(${component} ${type}
        THIRD_PARTY
            sfml
        SHARED_LIBRARY_SUFFIX
            -2
        LIBRARY_SUFFIX
            ${suffix}
        BINARY_DIR
            lib
        )
        list(APPEND sfml-targets sfml-${component})
    endforeach()
endif()

# We need opengl32 on windows for some Imgui stuff
if(WIN32)
    find_package(OpenGL REQUIRED)
    set(opengl ${OPENGL_LIBRARIES} opengl32)
endif()

add_siplasplas_library(imgui-sfml EXCLUDE_FROM_ALL
SOURCES
    imgui-sfml/imgui-SFML.cpp
    imgui-sfml/imgui.cpp
    imgui-sfml/imgui_draw.cpp
    imgui-sfml/imgui_demo.cpp
INCLUDE_DIRS
    imgui-sfml
    "${CMAKE_CURRENT_SOURCE_DIR}"
    ${SFML_INCLUDE_DIR}
DEPENDS
    ${sfml-targets}
    ${opengl}
)
