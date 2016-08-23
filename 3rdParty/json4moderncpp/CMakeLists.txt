if(NOT SIPLASPLAS_JSON4MODERNCPP_VERSION)
    set(SIPLASPLAS_JSON4MODERNCPP 1.1.0)
endif()

add_siplasplas_thirdparty(json4moderncpp HEADER_ONLY RENAME
URL
    "https://github.com/nlohmann/json/archive/v${SIPLASPLAS_JSON4MODERNCPP}.zip"
INCLUDE_DIRS
    ..
)
