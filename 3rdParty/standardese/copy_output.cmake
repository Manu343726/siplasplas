if(NOT SOURCE_DIR)
    message(FATAL_ERROR "No Standardese source dir given")
endif()
if(NOT OUTPUT_DIR)
    message(FATAL_ERROR "No Standardese output dir given")
endif()

file(GLOB_RECURSE files "${SOURCE_DIR}/*.md")

if(files)
    message(STATUS "Copying Standardese output markdown files to ${OUTPUT_DIR}...")
    file(COPY ${files} DESTINATION "${OUTPUT_DIR}")
endif()
