#ifndef POINT3_TESTING_H
#define POINT3_TESTING_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double current_time_seconds(void) {
    return (double)clock() / (double)CLOCKS_PER_SEC;
}

static void write_csv_header(FILE* file) {
    fprintf(file, "n,naive,binary,binomial,fibonacci\n");
}

static int distances_equal(const long long* a, const long long* b, int n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            return 0;
        }
    }

    return 1;
}

#endif
