add_siplasplas_thirdparty(ctti HEADER_ONLY
GIT_REPOSITORY
    "https://github.com/Manu343726/ctti"
INCLUDE_DIRS
    include/
)

if(MSVC)
    target_compile_definitions(ctti INTERFACE CTTI_STRING_MAX_LENGTH=512)
else()
    target_compile_definitions(ctti INTERFACE CTTI_STRING_MAX_LENGTH=1024)
endif()

