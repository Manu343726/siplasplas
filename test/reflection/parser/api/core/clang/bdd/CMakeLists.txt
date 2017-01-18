add_siplasplas_executable(reflection-parser-api-core-clang-bdd NAMESPACE tests
SOURCES
    diagnostics_test.cpp
    main.cpp
    visitors_test.cpp
DEPENDS
    siplasplas-reflection-parser-api-core-testutils
    siplasplas-reflection-parser-api-core-clang
    CONAN_PKG::catch
)
