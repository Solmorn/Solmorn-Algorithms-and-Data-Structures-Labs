#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long parse_ull(const char* s) {
    char* end = NULL;
    const unsigned long long value = strtoull(s, &end, 10);
    assert(end != s && *end == '\0');
    return value;
}

static void generate_uints(FILE* file, size_t n) {
    fprintf(file, "%zu\n", n);
    for (size_t i = 0; i < n; ++i) {
        const unsigned value = (unsigned)rand();
        fprintf(file, "%u\n", value);
    }
}

static float random_float_in_range(void) {
    const float t = (float)rand() / (float)RAND_MAX;
    return -10.0f + 20.0f * t;
}

static void generate_floats(FILE* file, size_t n) {
    fprintf(file, "%zu\n", n);
    for (size_t i = 0; i < n; ++i) {
        const float value = random_float_in_range();
        fprintf(file, "%a\n", value);
    }
}

static void generate_strings(FILE* file, size_t n) {
    fprintf(file, "%zu\n", n);
    char buffer[32];

    for (size_t i = 0; i < n; ++i) {
        const int len = 5 + rand() % 16;
        for (int j = 0; j < len; ++j) {
            buffer[j] = (char)('a' + rand() % 26);
        }
        buffer[len] = '\0';
        fprintf(file, "%s\n", buffer);
    }
}

int main(int argc, char** argv) {
    assert(argc == 5);

    const char* mode = argv[1];
    const size_t n = (size_t)parse_ull(argv[2]);
    const unsigned seed = (unsigned)parse_ull(argv[3]);
    const char* out_path = argv[4];

    FILE* file = fopen(out_path, "w");
    assert(file != NULL);

    srand(seed);

    if (strcmp(mode, "uint") == 0) {
        generate_uints(file, n);
    } else if (strcmp(mode, "float") == 0) {
        generate_floats(file, n);
    } else if (strcmp(mode, "string") == 0) {
        generate_strings(file, n);
    } else {
        assert(!"unknown generator mode");
    }

    fclose(file);
    return 0;
}
