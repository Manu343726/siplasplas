
function(install_siplasplas_library TARGET)
    install(TARGETS ${TARGET}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static
        RUNTIME DESTINATION bin
        COMPONENT libraries
    )

    headerdir_from_sourcetree(dir)
    install(DIRECTORY ${dir} DESTINATION include/siplasplas
        COMPONENT headers
    )
endfunction()

function(install_siplasplas_headeronly_library TARGET)
    headerdir_from_sourcetree(dir)
    install(DIRECTORY ${dir} DESTINATION include/siplasplas
        COMPONENT headers
    )
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
