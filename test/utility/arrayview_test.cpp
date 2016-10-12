#include <siplasplas/utility/array_view.hpp>

static_assert(
    cpp::constArrayView("hello") == cpp::constArrayView("hello"),
    "string literals must be equality comparable"
);
static_assert(
    cpp::constArrayView("hello") != cpp::constArrayView("world"),
    "string literals must be inequality comparable"
);
