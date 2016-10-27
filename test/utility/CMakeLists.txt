add_siplasplas_test(utility
SOURCES
    meta_test.cpp
    memory_manip_test.cpp
    compiles_test.cpp
    typeinfo_test.cpp
DEPENDS
    siplasplas-utility
    siplasplas-constexpr # To print typelists
                         # in meta test
DEFAULT_TEST_MAIN
)
