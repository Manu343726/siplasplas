add_siplasplas_thirdparty(spdlog HEADER_ONLY
GIT_REPOSITORY
    "https://github.com/gabime/spdlog"
INCLUDE_DIRS
    include/
DEPENDS
    fmt
)

target_compile_definitions(spdlog INTERFACE SPDLOG_FMT_EXTERNAL)
