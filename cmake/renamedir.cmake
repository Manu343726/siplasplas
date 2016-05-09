if(NOT EXISTS "${DEST}")
    message(STATUS "Renaming ${SOURCE} to ${DEST}")
    file(RENAME "${SOURCE}" "${DEST}")
endif()
