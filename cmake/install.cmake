
function(install_siplasplas_library TARGET)
    install(TARGETS ${TARGET}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        COMPONENT libraries
    )
endfunction()

function(install_siplasplas_headeronly_library TARGET)
endfunction()

function(install_siplasplas_example EXAMPLE)
    cmake_parse_arguments(
        "INSTALL"
        ""
        ""
        "SOURCES"
        ${ARGN}
    )

    install(TARGETS ${EXAMPLE}
        RUNTIME DESTINATION examples/bin
        COMPONENT examples
    )

    if(INSTALL_SOURCES)
        install(FILES ${INSTALL_SOURCES} DESTINATION examples/sources/${EXAMPLE}
            COMPONENT examples
        )
    endif()
endfunction()

install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/siplasplas" DESTINATION include/)
install(DIRECTORY "${CMAKE_BINARY_DIR}/output/reflection" DESTINATION include/)
install(DIRECTORY "${CMAKE_BINARY_DIR}/exports/siplasplas" DESTINATION include/)
install(DIRECTORY "${CMAKE_BINARY_DIR}/exports/examples" DESTINATION include/)
