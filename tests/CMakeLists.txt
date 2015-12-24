
include(cmake/gmock)

install_gtestgmock(VERSION 1.57.0)

gmock_test_target(NAME variant)
gmock_test_target(NAME linear_allocator)
