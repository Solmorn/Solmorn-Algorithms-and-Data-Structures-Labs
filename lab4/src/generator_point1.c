#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_size(const char* s, size_t* value) {
    char* end = NULL;
    unsigned long long parsed = 0u;

    errno = 0;
    parsed = strtoull(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0' || parsed > (unsigned long long)SIZE_MAX) {
        return 0;
    }

    *value = (size_t)parsed;
    return 1;
}

static int parse_unsigned(const char* s, unsigned* value) {
    char* end = NULL;
    unsigned long parsed = 0u;

    errno = 0;
    parsed = strtoul(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0' || parsed > UINT_MAX) {
        return 0;
    }

    *value = (unsigned)parsed;
    return 1;
}

static void generate_uints(FILE* file, size_t n) {
    fprintf(file, "%zu\n", n);
    for (size_t i = 0u; i < n; i++) {
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
    for (size_t i = 0u; i < n; i++) {
        const float value = random_float_in_range();
        fprintf(file, "%a\n", value);
    }
}

static void generate_strings(FILE* file, size_t n) {
    char buffer[32] = {0};

    fprintf(file, "%zu\n", n);
    for (size_t i = 0u; i < n; i++) {
        const int len = 5 + rand() % 16;
        for (int j = 0; j < len; j++) {
            buffer[j] = (char)('a' + rand() % 26);
        }
        buffer[len] = '\0';
        fprintf(file, "%s\n", buffer);
    }
}

int main(int argc, char** argv) {
    if (argc != 5) {
        fprintf(stderr, "usage: %s <uint|float|string> <count> <seed> <output>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* mode = argv[1];
    size_t n = 0u;
    unsigned seed = 0u;
    const char* out_path = argv[4];

    if (!parse_size(argv[2], &n)) {
        fprintf(stderr, "invalid count: %s\n", argv[2]);
        return EXIT_FAILURE;
    }
    if (!parse_unsigned(argv[3], &seed)) {
        fprintf(stderr, "invalid seed: %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    FILE* file = fopen(out_path, "w");
    if (file == NULL) {
        perror(out_path);
        return EXIT_FAILURE;
    }

    srand(seed);

    if (strcmp(mode, "uint") == 0) {
        generate_uints(file, n);
    } else if (strcmp(mode, "float") == 0) {
        generate_floats(file, n);
    } else if (strcmp(mode, "string") == 0) {
        generate_strings(file, n);
    } else {
        fprintf(stderr, "unknown generator mode: %s\n", mode);
        fclose(file);
        return EXIT_FAILURE;
    }

    if (fclose(file) != 0) {
        perror(out_path);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
