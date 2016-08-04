find_package(Git REQUIRED)


function(git)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} ${ARGN}
        OUTPUT_VARIABLE output
    )
    string(STRIP "${output}" output)

    set(GIT_OUTPUT ${output} PARENT_SCOPE)
endfunction()

function(get_git_branch RESULT)
    git(rev-parse --abbrev-ref HEAD)
    set(${RESULT} ${GIT_OUTPUT} PARENT_SCOPE)
endfunction()

function(get_git_commit_hash RESULT)
    git(rev-parse --short HEAD)
    set(${RESULT} ${GIT_OUTPUT} PARENT_SCOPE)
endfunction()

function(get_git_commit_message RESULT)
    git(log -1 --pretty=%s)
    set(${RESULT} ${GIT_OUTPUT} PARENT_SCOPE)
endfunction()

if(NOT SIPLASPLAS_GIT_BRANCH)
    get_git_branch(SIPLASPLAS_GIT_BRANCH)
endif()
if(NOT SIPLASPLAS_GIT_COMMIT_HASH)
    get_git_commit_hash(SIPLASPLAS_GIT_COMMIT_HASH)
endif()
if(NOT SIPLASPLAS_GIT_COMMIT_MESSAGE)
    get_git_commit_message(SIPLASPLAS_GIT_COMMIT_MESSAGE)
endif()

message(STATUS "siplasplas git branch: ${SIPLASPLAS_GIT_BRANCH}")
message(STATUS "siplasplas git commit: ${SIPLASPLAS_GIT_COMMIT_HASH}: \"${SIPLASPLAS_GIT_COMMIT_MESSAGE}\"")
