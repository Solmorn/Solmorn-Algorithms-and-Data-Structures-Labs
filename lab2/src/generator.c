#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint64_t splitmix64_next(uint64_t* state) {
    // SplitMix64 default constants are used
    uint64_t z = (*state += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

static uint64_t random_bounded(uint64_t* state, uint64_t bound) {
    assert(bound > 0);

    uint64_t limit = UINT64_MAX - (UINT64_MAX % bound);
    uint64_t value = 0;

    do {
        value = splitmix64_next(state);
    } while (value >= limit);

    return value % bound;
}

static unsigned long long parse_ull(const char* s) {
    char* end = NULL;
    unsigned long long value = strtoull(s, &end, 10);
    assert(end != s && *end == '\0');
    return value;
}

int main(int argc, char** argv) {
    assert(argc == 3);

    size_t n = (size_t)parse_ull(argv[1]);
    unsigned long long max_value = parse_ull(argv[2]);

    assert(max_value <= INT_MAX);

    uint64_t state = (uint64_t)time(NULL) ^ ((uint64_t)clock() << 32);

    printf("%zu\n", n);

    for (size_t i = 0; i < n; ++i) {
        int value = (int)random_bounded(&state, (uint64_t)max_value + 1ULL);
        printf("%d", value);
        if (i + 1 < n) {
            putchar(' ');
        }
    }

    putchar('\n');
    return 0;
}