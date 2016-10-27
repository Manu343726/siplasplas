#include <siplasplas/constexpr/algorithm.hpp>
#include <array>

using namespace ::cpp::constexp;

static_assert(min(0, 1) == 0,       "min(int, int) failed");
static_assert(min(0, 1, 2) == 0,    "min(int, int, int) failed");
static_assert(min(0, 1, 2, 3) == 0, "min(int, int, int, int) failed");

static_assert(max(0, 1) == 1,       "max(int, int) failed");
static_assert(max(0, 1, 2) == 2,    "max(int, int, int) failed");
static_assert(max(0, 1, 2, 3) == 3, "max(int, int, int, int) failed");

constexpr char hello[] = {'h', 'e', 'l', 'l', 'o', '\0'};
constexpr char hola[]  = {'h', 'o', 'l', 'a', '\0'};
constexpr char world[] = {'w', 'o', 'r', 'l', 'd', '\0'};

constexpr char kitten[]  = {'k', 'i', 't', 't', 'e', 'n', '\0'};
constexpr char sitting[] = {'s', 'i', 't', 't', 'i', 'n', 'g', '\0'};

static_assert(
    equal(
        begin(hello), end(hello),
        begin(hello), end(hello)
    ),
    "equal(array, array) failed"
);

static_assert(
    !equal(
        begin(hello), end(hello),
        begin(world), end(world)
    ),
    "!equal(array, array) failed"
);

static_assert(
    distance(
        begin(hello),
        end(hello)
    ) == 6,
    "distance(it, it) failed"
);

// From the wikipedia page on the
// Levenshteinn algorithm:
static_assert(
    levenshteinDistance(
        "kitten",
        "sitting"
    ) == 3,
    "levenshteinDistance(kitten, sitting) failed"
);
