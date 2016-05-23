if(NOT EXISTS "${DEST}")
    message(STATUS "Renaming ${SOURCE} to ${DEST}")
    file(COPY "${SOURCE}" "${DEST}")
endif()
