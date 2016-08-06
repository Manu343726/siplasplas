# Until Standardese had templating support,
# post-process Standardese output markdown
# to add an YAML front matter needed by Jekyll

message(STATUS "Stadndardese docs root dir: ${ROOT_DIR}")
message(STATUS "Current branch: ${BRANCH}")
message(STATUS "Sanitized branch name: ${SANITIZED_BRANCH}")
message(STATUS "Branch dir: ${BRANCH_DIR}")

set(doc_url "{{site.url}}{{site.baseurl}}/doc")
set(standardese_doc_url "${doc_url}/standardese")
set(branch_doc_url "${standardese_doc_url}/${SANITIZED_BRANCH}")
set(standardese_doc_front_matter "---
layout: standardese-doc
---

")

# Generate and index file for the doc/standardese page linking to
# the different documented branches.
# I would liked to do this using Jekyll templates instead, but it
# seems to not support file IO. (There are some plugins though, but I feel
# more confident with CMake scripting than introducing a third-party plugin
# in the jekyll engine. See https://github.com/sillylogger/jekyll-directory)
function(generate_doc_index)
    message(STATUS "Generating documentation index...")

    file(GLOB branches RELATIVE "${ROOT_DIR}" "${ROOT_DIR}/*")

    set(index "${standardese_doc_front_matter}
# siplasplas documentation

")

    foreach(branch ${branches})
        if(IS_DIRECTORY "${ROOT_DIR}/${branch}")
            # Get back the branch name from the encoded directory name:
            set(sanitized_branch "${branch}")
            string(REGEX REPLACE "__slash__" "/" branch "${branch}")

            message(STATUS " - branch '${branch}'")

            set(index "${index}
 - [`${branch}` branch documentation](${standardese_doc_url}/${sanitized_branch}/)")
        endif()
    endforeach()

    file(WRITE "${ROOT_DIR}/index.md" "${index}")
endfunction()

# Process branch documentation, processing each module output
# and generating an index file for the page:
function(process_branch)
    message(STATUS "Processing '${BRANCH}' branch documentation:")

    file(GLOB modules RELATIVE "${BRANCH_DIR}" "${BRANCH_DIR}/*")

    set(index "${standardese_doc_front_matter}
# siplasplas `${BRANCH}` branch documentation

")

    foreach(module ${modules})
        if(IS_DIRECTORY "${BRANCH_DIR}/${module}")
            process_docs("${module}")

            set(index "${index}
 - [`${module}` module](${branch_doc_url}/${module}/)")
        endif()
    endforeach()

    message(STATUS "Writing '${BRANCH}' branch index file")
    file(WRITE "${BRANCH_DIR}/index.md" "${index}")
endfunction()

function(process_docs MODULE)
    message(STATUS "  Processing '${MODULE}' module docs...")
    file(GLOB_RECURSE files "${BRANCH_DIR}/${MODULE}/*.md")

    set(index "${standardese_doc_front_matter}
# `${MODULE}` module files

")

    foreach(file ${files})
        get_filename_component(name "${file}" NAME)
        get_filename_component(name_we "${file}" NAME_WE)
        message(STATUS "   - generating ${name} front matter")

        file(READ "${file}" file_text)
        set(file_text "${standardese_doc_front_matter}${file_text}")
        file(WRITE "${file}" "${file_text}")

        set(index "${index}
 - [`${name_we}.hpp`](${branch_doc_url}/${MODULE}/${name_we}.html)")
    endforeach()

    message(STATUS "  Writing '${MODULE}' index file")
    file(WRITE "${BRANCH_DIR}/${MODULE}/index.md" "${index}")
endfunction()

generate_doc_index()
process_branch()
