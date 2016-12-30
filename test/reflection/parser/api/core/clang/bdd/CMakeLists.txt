add_siplasplas_executable(reflection-parser-api-core-clang-bdd NAMESPACE tests
SOURCES
    diagnostics_test.cpp
    main.cpp
DEPENDS
    siplasplas-reflection-parser-api-core-clang-test
    CONAN_PKG::catch
)
