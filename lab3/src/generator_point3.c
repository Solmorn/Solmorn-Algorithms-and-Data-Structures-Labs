#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned long long parse_ull(const char* s) {
    char* end = NULL;
    unsigned long long value = strtoull(s, &end, 10);

    assert(end != s && *end == '\0');
    return value;
}

static void write_int(int value) {
    assert(fwrite(&value, sizeof(int), 1, stdout) == 1);
}

static size_t edge_key(int n, int u, int v) {
    return (size_t)u * (size_t)n + (size_t)v;
}

static int bit_get(unsigned char* bits, size_t index) {
    return (bits[index >> 3] >> (index & 7)) & 1u;
}

static void bit_set(unsigned char* bits, size_t index) {
    bits[index >> 3] |= (unsigned char)(1u << (index & 7));
}

static void mark_undirected_edge(unsigned char* bits, int n, int u, int v) {
    bit_set(bits, edge_key(n, u, v));
    bit_set(bits, edge_key(n, v, u));
}

static int random_weight(int max_weight) {
    return rand() % (max_weight + 1);
}

static void generate_sparse_graph(int n, int max_weight) {
    int m = 4 * n;
    if (m < n - 1) {
        m = n - 1;
    }

    write_int(n);
    write_int(m);

    size_t bit_count = (size_t)n * (size_t)n;
    unsigned char* used = (unsigned char*)calloc((bit_count + 7) / 8, sizeof(unsigned char));
    assert(used != NULL);

    int written = 0;

    for (int i = 0; i + 1 < n; ++i) {
        int u = i;
        int v = i + 1;
        int w = random_weight(max_weight);

        mark_undirected_edge(used, n, u, v);

        write_int(u);
        write_int(v);
        write_int(w);

        written++;
    }

    while (written < m) {
        int u = rand() % n;
        int v = rand() % n;

        if (u == v) continue;
        if (bit_get(used, edge_key(n, u, v))) continue;

        int w = random_weight(max_weight);

        mark_undirected_edge(used, n, u, v);

        write_int(u);
        write_int(v);
        write_int(w);

        written++;
    }

    free(used);
}

static void generate_dense_graph(int n, int max_weight) {
    int m = n * (n - 1) / 2;

    write_int(n);
    write_int(m);

    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            int w = random_weight(max_weight);

            write_int(u);
            write_int(v);
            write_int(w);
        }
    }
}

int main(int argc, char** argv) {
    assert(argc == 5);

    const char* mode = argv[1];
    int n = (int)parse_ull(argv[2]);
    int max_weight = (int)parse_ull(argv[3]);
    unsigned seed = (unsigned)parse_ull(argv[4]);

    srand(seed);

    if (strcmp(mode, "sparse") == 0) {
        generate_sparse_graph(n, max_weight);
    } else {
        assert(strcmp(mode, "dense") == 0);
        generate_dense_graph(n, max_weight);
    }

    return 0;
}