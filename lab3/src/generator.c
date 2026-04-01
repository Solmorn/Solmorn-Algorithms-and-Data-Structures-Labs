#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static unsigned long long parse_ull(const char* s) {
    char* end = NULL;
    unsigned long long value = strtoull(s, &end, 10);

    assert(end != s && *end == '\0');
    return value;
}

int main(int argc, char** argv) {
    assert(argc == 4);

    size_t n = (size_t)parse_ull(argv[1]);
    int max_value = (int)parse_ull(argv[2]);
    unsigned seed = (unsigned)parse_ull(argv[3]);

    assert(max_value >= 0);
    srand(seed);

    printf("%zu\n", n);

    for (size_t i = 0; i < n; ++i) {
        int value = rand() % (max_value + 1);
        printf("%d", value);

        if (i + 1 < n) {
            putchar(' ');
        }
    }

    putchar('\n');
    return 0;
}