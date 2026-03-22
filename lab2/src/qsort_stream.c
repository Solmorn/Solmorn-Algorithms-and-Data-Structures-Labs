#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int int_cmp(const void* lhs, const void* rhs) {
    int a = *(const int*)lhs;
    int b = *(const int*)rhs;

    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int main(void) {
    size_t n = 0;
    int* arr = NULL;

    assert(scanf("%zu", &n) == 1);

    if (n > 0) {
        arr = (int*)malloc(n * sizeof(int));
        assert(arr != NULL);
    }

    for (size_t i = 0; i < n; ++i) {
        assert(scanf("%d", &arr[i]) == 1);
    }

    qsort(arr, n, sizeof(int), int_cmp);

    printf("%zu\n", n);
    for (size_t i = 0; i < n; ++i) {
        printf("%d", arr[i]);
        if (i + 1 < n) {
            putchar(' ');
        }
    }
    putchar('\n');

    free(arr);
    return 0;
}