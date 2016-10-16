#include <siplasplas/utility/constexpralgorithms.hpp>
#include <array>

static_assert(cpp::min(0, 1) == 0,       "cpp::min(int, int) failed");
static_assert(cpp::min(0, 1, 2) == 0,    "cpp::min(int, int, int) failed");
static_assert(cpp::min(0, 1, 2, 3) == 0, "cpp::min(int, int, int, int) failed");

static_assert(cpp::max(0, 1) == 1,       "cpp::max(int, int) failed");
static_assert(cpp::max(0, 1, 2) == 2,    "cpp::max(int, int, int) failed");
static_assert(cpp::max(0, 1, 2, 3) == 3, "cpp::max(int, int, int, int) failed");

constexpr char hello[] = {'h', 'e', 'l', 'l', 'o', '\0'};
constexpr char hola[]  = {'h', 'o', 'l', 'a', '\0'};
constexpr char world[] = {'w', 'o', 'r', 'l', 'd', '\0'};

constexpr char kitten[]  = {'k', 'i', 't', 't', 'e', 'n', '\0'};
constexpr char sitting[] = {'s', 'i', 't', 't', 'i', 'n', 'g', '\0'};

static_assert(
    cpp::equal(
        cpp::begin(hello), cpp::end(hello),
        cpp::begin(hello), cpp::end(hello)
    ),
    "cpp::equal(array, array) failed"
);

static_assert(
    !cpp::equal(
        cpp::begin(hello), cpp::end(hello),
        cpp::begin(world), cpp::end(world)
    ),
    "!cpp::equal(array, array) failed"
);

static_assert(
    cpp::distance(
        cpp::begin(hello),
        cpp::end(hello)
    ) == 6,
    "cpp::distance(it, it) failed"
);

// From the wikipedia page on the
// Levenshteinn algorithm:
static_assert(
    cpp::levenshteinDistance(
        "kitten",
        "sitting"
    ) == 3,
    "cpp::levenshteinDistance(kitten, sitting) failed"
);
