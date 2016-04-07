add_library(ctti INTERFACE)
target_include_directories(ctti INTERFACE ${CMAKE_SOURCE_DIR}/3rdParty/ctti/include)

if(MSVC)
    target_compile_definitions(ctti INTERFACE -DCTTI_STRING_MAX_LENGTH=512)
else()
    target_compile_definitions(ctti INTERFACE -DCTTI_STRING_MAX_LENGTH=1024)
endif()

