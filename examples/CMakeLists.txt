
if(NOT MSVC)
	set(CPP_example_ALL_COMPILE_OPTIONS -std=c++14 -Wall -pedantic -ftemplate-depth-1024)
	set(CPP_example_DEBUG_COMPILE_OPTIONS -O0 -g3 -fstack-protector-strong)
	set(CPP_example_RELEASE_COMPILE_OPTIONS -O3 -g0)
endif()

exec_target(NAME variant PREFIX example)
exec_target(NAME gamelike_variant PREFIX example)

if(example_variant)
    if(MSVC)
        # We all love MSVC constexpr support...
        target_compile_definitions(example_variant PRIVATE CTTI_STRING_MAX_LENGTH=512)
    else()
        target_compile_definitions(example_variant PRIVATE CTTI_STRING_MAX_LENGTH=1024)
    endif()
endif()
