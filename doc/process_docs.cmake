# Until Standardese had templating support,
# post-process Standardese output markdown
# to add an YAML front matter needed by Jekyll

message(STATUS "Docs root dir: ${DOC_DIR}")
message(STATUS "Current branch: ${BRANCH}")
message(STATUS "Sanitized branch name: ${SANITIZED_BRANCH}")
message(STATUS "Doxygen branch dir: ${DOXYGEN_BRANCH_DIR}")
message(STATUS "Siplasplas branch dir: ${SIPLASPLAS_BRANCH_DIR}")

set(doc_url "{{site.url}}{{site.baseurl}}/doc")
set(doxygen_doc_url     "${doc_url}/doxygen")
set(standardese_doc_url "${doc_url}/standardese")
set(doxygen_branch_doc_url     "${doxygen_doc_url}/${DSANITIZED_BRANCH}")
set(standardese_branch_doc_url "${standardese_doc_url}/${SANITIZED_BRANCH}")
set(standardese_doc_front_matter "---
layout: standardese-doc
---

")
set(doxygen_doc_front_matter "---
layout: doxygen-doc
---

")

# Generate and index file for the doc/standardese page linking to
# the different documented branches.
# I would liked to do this using Jekyll templates instead, but it
# seems to not support file IO. (There are some plugins though, but I feel
# more confident with CMake scripting than introducing a third-party plugin
# in the jekyll engine. See https://github.com/sillylogger/jekyll-directory)
function(generate_doc_index TOOL)
    message(STATUS "Generating ${TOOL} documentation index...")

    file(GLOB branches RELATIVE "${DOC_DIR}/${TOOL}" "${DOC_DIR}/${TOOL}/*")

    set(index "${${TOOL}_doc_front_matter}
# siplasplas ${TOOL} documentation

")

    foreach(branch ${branches})
        if(IS_DIRECTORY "${DOC_DIR}/${TOOL}/${branch}")
            # Get back the branch name from the encoded directory name:
            set(sanitized_branch "${branch}")
            string(REGEX REPLACE "__slash__" "/" branch "${branch}")

            message(STATUS " - branch '${branch}'")

            set(index "${index}
 - [`${branch}` branch documentation](${${TOOL}_doc_url}/${sanitized_branch}/)")
        endif()
    endforeach()

    file(WRITE "${DOC_DIR}/${TOOL}/index.md" "${index}")
endfunction()

# Process branch documentation, processing each module output
# and generating an index file for the page:
function(process_branch)
    message(STATUS "Processing '${BRANCH}' branch documentation:")

    file(GLOB modules RELATIVE "${STANDARDESE_BRANCH_DIR}" "${STANDARDESE_BRANCH_DIR}/*")

    set(index "${standardese_doc_front_matter}
# siplasplas `${BRANCH}` branch documentation

")

    foreach(module ${modules})
        if(IS_DIRECTORY "${STANDARDESE_BRANCH_DIR}/${module}")
            process_docs("${module}")

            set(index "${index}
 - [`${module}` module](${standardese_branch_doc_url}/${module}/)")
        endif()
    endforeach()

    message(STATUS "Writing standardese '${BRANCH}' branch index file")
    file(WRITE "${STANDARDESE_BRANCH_DIR}/index.md" "${index}")
endfunction()

function(process_docs MODULE)
    message(STATUS "  Processing '${MODULE}' module docs...")
    file(GLOB_RECURSE files "${STANDARDESE_BRANCH_DIR}/${MODULE}/*.md")

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
 - [`${name_we}.hpp`](${standardese_branch_doc_url}/${MODULE}/${name_we}.html)")
    endforeach()

    message(STATUS "  Writing '${MODULE}' index file")
    file(WRITE "${STANDARDESE_BRANCH_DIR}/${MODULE}/index.md" "${index}")
endfunction()

generate_doc_index(doxygen)
generate_doc_index(standardese)
process_branch()
