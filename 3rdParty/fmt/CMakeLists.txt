if(NOT SIPLASPLAS_FMT_VERSION)
    set(SIPLASPLAS_FMT_VERSION 3.0.0)
endif()

add_siplasplas_thirdparty(fmt
URL
    "https://github.com/fmtlib/fmt/releases/download/${SIPLASPLAS_FMT_VERSION}/fmt-${SIPLASPLAS_FMT_VERSION}.zip"
COMPILE_OPTIONS
    -fPIC
CMAKE_EXTRA_ARGS
    -DCMAKE_VERBOSE_MAKEFILE=ON
    -DFMT_TEST=OFF
HEADER_DIRS
    fmt
)

add_siplasplas_thirdparty_component(fmt DEFAULT
THIRD_PARTY
    fmt
BINARY_DIR
    fmt
)
