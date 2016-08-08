if(NOT MSVC)
	set(fPIC -fPIC)
endif()

add_siplasplas_thirdparty(efsw
HG_REPOSITORY
    "https://bitbucket.org/SpartanJ/efsw"
CMAKE_EXTRA_ARGS
	-DSTATIC_LIB=ON
COMPILE_OPTIONS
	${fPIC}
)

add_siplasplas_thirdparty_component(efsw DEFAULT
THIRD_PARTY
    efsw
INCLUDE_DIRS
    include/
)

find_package(Threads REQUIRED)

target_link_libraries(efsw INTERFACE ${CMAKE_THREAD_LIBS_INIT})
