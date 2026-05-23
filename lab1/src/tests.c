#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arr_stack.h"
#include "list_stack.h"
#include "tests.h"

#define STEP 1000
#define MAX_N 1000000
#define ELEM_SIZE sizeof(int)

int* CreateData(int n) {
    int* data = (int*)calloc(n, sizeof(int));
    if (!data) return NULL;

    for (int i = 0; i < n; i++) {
        data[i] = rand();
    }

    return data;
}

double TestOne_arr_stack(int* data) {
    ArrStack* st = arr_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;
    int n = 1000000;

    if (!st || !data) return 0.0;

    clock_t t0 = clock();

    for (int i = 0; i < n; i++) {
        arr_stack_push(st, &data[data_index]);
        data_index++;
    }

    while (n >= 100000) {
        for (int i = 0; i < n / 2; i++) arr_stack_pop(st);

        n /= 2;

        for (int i = 0; i < n / 2; i++) {
            arr_stack_push(st, &data[data_index]);
            data_index++;
        }

        n += n / 2;
    }

    clock_t t1 = clock();

    arr_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

double TestOne_list_stack(int* data) {
    ListStack* st = list_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;
    int n = 1000000;

    if (!st || !data) return 0.0;

    clock_t t0 = clock();

    for (int i = 0; i < n; i++) {
        list_stack_push(st, &data[data_index]);
        data_index++;
    }

    while (n >= 100000) {
        for (int i = 0; i < n / 2; i++) list_stack_pop(st);

        n /= 2;

        for (int i = 0; i < n / 2; i++) {
            list_stack_push(st, &data[data_index]);
            data_index++;
        }

        n += n / 2;
    }

    clock_t t1 = clock();

    list_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

double TestTwo_arr_stack(int* data) {
    ArrStack* st = arr_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;
    int n = 1000000;

    if (!st || !data) return 0.0;

    clock_t t0 = clock();

    for (int i = 0; i < n; i++) {
        arr_stack_push(st, &data[data_index]);
        data_index++;
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10000; j++) arr_stack_pop(st);

        for (int j = 0; j < 10000; j++) {
            arr_stack_push(st, &data[data_index]);
            data_index++;
        }
    }

    while (n >= 100000) {
        for (int i = 0; i < n / 2; i++) arr_stack_pop(st);

        n /= 2;

        for (int i = 0; i < n / 2; i++) {
            arr_stack_push(st, &data[data_index]);
            data_index++;
        }

        n += n / 2;
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10000; j++) arr_stack_pop(st);

        for (int j = 0; j < 10000; j++) {
            arr_stack_push(st, &data[data_index]);
            data_index++;
        }
    }

    clock_t t1 = clock();

    arr_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

double TestTwo_list_stack(int* data) {
    ListStack* st = list_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;
    int n = 1000000;

    if (!st || !data) return 0.0;

    clock_t t0 = clock();

    for (int i = 0; i < n; i++) {
        list_stack_push(st, &data[data_index]);
        data_index++;
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10000; j++) list_stack_pop(st);

        for (int j = 0; j < 10000; j++) {
            list_stack_push(st, &data[data_index]);
            data_index++;
        }
    }

    while (n >= 100000) {
        for (int i = 0; i < n / 2; i++) list_stack_pop(st);

        n /= 2;

        for (int i = 0; i < n / 2; i++) {
            list_stack_push(st, &data[data_index]);
            data_index++;
        }

        n += n / 2;
    }

    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 10000; j++) list_stack_pop(st);

        for (int j = 0; j < 10000; j++) {
            list_stack_push(st, &data[data_index]);
            data_index++;
        }
    }

    clock_t t1 = clock();

    list_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

double TestThree_arr_stack(int* data_values, int* data_instructions) {
    ArrStack* st = arr_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;
    int n = 1000000;

    if (!st || !data_values || !data_instructions) return 0.0;

    for (int i = 0; i < n; i++) {
        arr_stack_push(st, &data_values[data_index]);
        data_index++;
    }

    clock_t t0 = clock();

    for (int i = 0; i < 1000000; i++) {
        if ((data_instructions[i] % 2) == 0) {
            arr_stack_push(st, &data_values[data_index]);
            data_index++;
        } else {
            arr_stack_pop(st);
        }
    }

    clock_t t1 = clock();

    arr_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

double TestThree_list_stack(int* data_values, int* data_instructions) {
    ListStack* st = list_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;
    int n = 1000000;

    if (!st || !data_values || !data_instructions) return 0.0;

    for (int i = 0; i < n; i++) {
        list_stack_push(st, &data_values[data_index]);
        data_index++;
    }

    clock_t t0 = clock();

    for (int i = 0; i < 1000000; i++) {
        if ((data_instructions[i] % 2) == 0) {
            list_stack_push(st, &data_values[data_index]);
            data_index++;
        } else {
            list_stack_pop(st);
        }
    }

    clock_t t1 = clock();

    list_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

static double TestFour_arr_stack_push_time(int n, int* data) {
    ArrStack* st = arr_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;

    if (!st || !data) return 0.0;

    clock_t t0 = clock();

    for (int i = 0; i < n; i++) {
        arr_stack_push(st, &data[data_index]);
        data_index++;
    }

    clock_t t1 = clock();

    arr_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

static double TestFour_list_stack_push_time(int n, int* data) {
    ListStack* st = list_stack_ctr(ELEM_SIZE);
    size_t data_index = 0;

    if (!st || !data) return 0.0;

    clock_t t0 = clock();

    for (int i = 0; i < n; i++) {
        list_stack_push(st, &data[data_index]);
        data_index++;
    }

    clock_t t1 = clock();

    list_stack_dtr(st);

    return (double)(t1 - t0) / CLOCKS_PER_SEC;
}

int TestFour_both_stack(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) return 0;

    fprintf(f, "n arr_time list_time\n");

    for (int n = STEP; n <= MAX_N; n += STEP) {
        int* data = CreateData(n);
        if (!data) {
            fclose(f);
            return 0;
        }

        double arr_time = TestFour_arr_stack_push_time(n, data);
        double list_time = TestFour_list_stack_push_time(n, data);

        free(data);

        fprintf(f, "%d %.9f %.9f\n", n, arr_time, list_time);
    }

    fclose(f);

    return 1;
}
