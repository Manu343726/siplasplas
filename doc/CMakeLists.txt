# Adapted from Louis Dionne's Boost.Hana config

find_package(Doxygen REQUIRED)
find_package(Git REQUIRED)


add_custom_target(doc
    COMMAND ${CMAKE_COMMAND} -E remove_directory html
    COMMAND ! ${DOXYGEN_EXECUTABLE} Doxyfile 2>&1 | grep -E "warning|error"
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
)

if(SIPLASPLAS_DEPLOY_DOCS)
    set(SIPLASPLAS_DOXYGEN_QUIET YES)
    set(SIPLASPLAS_DOXYGEN_WARNINGS NO)

    if(NOT SIPLASPLAS_GIT_BRANCH)
        message(FATAL_ERROR "No siplasplas git branch set")
    endif()
    if(NOT SIPLASPLAS_GIT_COMMIT_HASH)
        message(FATAL_ERROR "No siplasplas git commit hash set")
    endif()
    if(NOT SIPLASPLAS_GIT_COMMIT_MESSAGE)
        set(SIPLASPLAS_GIT_COMMIT_MESSAGE "(empty message)")
    endif()
    if(NOT SIPLASPLAS_GITHUB_USER)
        message(FATAL_ERROR "No github user for docs publishing")
    endif()
    if(NOT SIPLASPLAS_GITHUB_USER_TOKEN)
        message(FATAL_ERROR "No github user token for docs publishing")
    endif()
    set(COMMIT_BRIEF "${SIPLASPLAS_GIT_BRANCH} ${SIPLASPLAS_GIT_COMMIT_HASH}: \"${SIPLASPLAS_GIT_COMMIT_MESSAGE}\"")

    set(SIPLASPLAS_REPO_URL "https://${SIPLASPLAS_GITHUB_USER}:${SIPLASPLAS_GITHUB_USER_TOKEN}@github.com/Manu343726/siplasplas")
    set(SIPLASPLAS_WEBSITE_PATH "${CMAKE_BINARY_DIR}/siplasplas_website")

    # Avoid creating extra directories for a branch that uses slashes like the feature
    # branches of gitflow
    string(REGEX REPLACE "/" "__slash__" sanitized_branch "${SIPLASPLAS_GIT_BRANCH}")


    add_custom_target(deploy-docs_clone_website
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${SIPLASPLAS_WEBSITE_PATH}"
        COMMAND ${GIT_EXECUTABLE} clone ${SIPLASPLAS_REPO_URL} "${SIPLASPLAS_WEBSITE_PATH}" -b gh-pages --depth 1
        COMMENT "Cloning siplasplas website"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        VERBATIM
    )

    # Automatically generate docs before cloning website
    add_dependencies(deploy-docs_clone_website doc)

    set(standardese_docs_src  "${CMAKE_BINARY_DIR}/doc/standardese")
    set(standardese_docs_dest "${SIPLASPLAS_WEBSITE_PATH}/doc/standardese/${sanitized_branch}")
    set(doxygen_docs_src  "${CMAKE_BINARY_DIR}/doc/html")
    set(doxygen_docs_dest "${SIPLASPLAS_WEBSITE_PATH}/doc/doxygen/${sanitized_branch}")

    if(SIPLASPLAS_DEPLOY_STANDARDESE_DOCS)
        # Automatically generate standardese docs before cloning website
        if(NOT SIPLASPLAS_DEPLOY_STANDARDESE_DOCS_POSTPROCESS_ONLY)
            add_dependencies(deploy-docs_clone_website standardese-doc)
        endif()

        set(standardese_clean
            COMMAND ${CMAKE_COMMAND} -E remove_directory "${standardese_docs_dest}"
        )
        set(standardese_copy
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${standardese_docs_src}" "${standardese_docs_dest}"
            COMMAND ${CMAKE_COMMAND} -DROOT_DIR=${SIPLASPLAS_WEBSITE_PATH}/doc/standardese/ -DBRANCH=${SIPLASPLAS_GIT_BRANCH} -DSANITIZED_BRANCH=${sanitized_branch} -DBRANCH_DIR=${standardese_docs_dest} -P ${CMAKE_SOURCE_DIR}/doc/process_standardese_output.cmake
        )
    endif()

    add_custom_target(deploy-docs
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${doxygen_docs_dest}"
        ${standardese_clean}
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${doxygen_docs_src}" "${doxygen_docs_dest}"
        ${standardese_copy}
        COMMAND ${GIT_EXECUTABLE} add --all doc
        COMMAND ${GIT_EXECUTABLE} commit --allow-empty -m "Updated docs for ${COMMIT_BRIEF}"
        COMMAND ${GIT_EXECUTABLE} push origin gh-pages --quiet # Push quietly to not leak the access token
        COMMENT "Publishing docs for ${COMMIT_BRIEF}"
        WORKING_DIRECTORY "${SIPLASPLAS_WEBSITE_PATH}"
        VERBATIM
    )

    # Clone website before committing changes
    add_dependencies(deploy-docs deploy-docs_clone_website)
else()
    set(SIPLASPLAS_DOXYGEN_QUIET NO)
    set(SIPLASPLAS_DOXYGEN_WARNINGS YES)

    message(STATUS "Docs deployment targets disabled")
endif()

configure_file(Doxyfile.in Doxyfile)
