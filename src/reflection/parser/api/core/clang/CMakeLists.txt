include(libclang)

add_siplasplas_library(siplasplas-reflection-parser-api-core-clang
SOURCES
    translationunit.cpp
    index.cpp
    string.cpp
    error.cpp
    unsavedfile.cpp
    diagnostic.cpp
    logger.cpp
    compileoptions.cpp
    cursor.cpp
    cursorkind.cpp
    visitor.cpp
    visitorinterface.cpp
    recursivevisitor.cpp
    file.cpp
    sourcelocation.cpp
DEPENDS
    siplasplas-utility
    siplasplas-constexpr
    CONAN_PKG::clang
    CONAN_PKG::type_safe
    clang-conan-clang
)

target_compile_definitions(siplasplas-reflection-parser-api-core-clang PUBLIC
    -DSIPLASPLAS_LIBCLANG_VERSION="${SIPLASPLAS_LIBCLANG_VERSION}"
    -DSIPLASPLAS_LIBCLANG_LIBRARY="${SIPLASPLAS_LIBCLANG_LIBRARY}"
    -DSIPLASPLAS_LIBCLANG_INCLUDE_DIR="${SIPLASPLAS_LIBCLANG_INCLUDE_DIR}"
    -DSIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR="${SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR}"
)

message(STATUS "libclang version: ${SIPLASPLAS_LIBCLANG_VERSION} (${SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR})")
message(STATUS "libclang library: ${SIPLASPLAS_LIBCLANG_LIBRARY}")
message(STATUS "libclang include dir: ${SIPLASPLAS_LIBCLANG_INCLUDE_DIR}")
message(STATUS "libclang system include dir: ${SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR}")
