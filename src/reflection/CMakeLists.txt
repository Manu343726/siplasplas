add_subdirectory(common)
add_subdirectory(static)
add_subdirectory(dynamic)
add_subdirectory(parser)

if(NOT SIPLASPLAS_REFLECTION_MODULE)
    set(SIPLASPLAS_REFLECTION_MODULE "${CMAKE_CURRENT_LIST_DIR}/parser/drlparser.cmake" CACHE INTERNAL "")
endif()

install(DIRECTORY parser DESTINATION bin/reflection-parser
    COMPONENT tools
    PATTERN *.pyc EXCLUDE
)

install(FILES parser/DRLParser DESTINATION bin/reflection-parser/parser
PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_WRITE GROUP_EXECUTE
)
