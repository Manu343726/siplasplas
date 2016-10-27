#include <siplasplas/constexpr/arrayview.hpp>
#include <gmock/gmock.h>
using namespace ::testing;
using namespace ::cpp::constexp;

static_assert(
    constArrayView("hello") == constArrayView("hello"),
    "string literals must be equality comparable"
);
static_assert(
    constArrayView("hello") != constArrayView("world"),
    "string literals must be inequality comparable"
);
