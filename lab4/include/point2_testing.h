#ifndef POINT2_TESTING_H
#define POINT2_TESTING_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define POINT2_INSERT_OP 0
#define POINT2_FIND_OP   1
#define POINT2_ERASE_OP  2

static inline double point2_now_seconds(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}

static inline uint32_t point2_rand_u32(void) {
    uint32_t a = (uint32_t)(rand() & 0x7fff);
    uint32_t b = (uint32_t)(rand() & 0x7fff);
    uint32_t c = (uint32_t)(rand() & 0x7fff);
    return (a << 17) ^ (b << 2) ^ c;
}

static inline int point2_rand_int(void) {
    return (int)point2_rand_u32();
}

static inline int point2_choose_operation(double p_insert, double p_find, double p_erase) {
    (void)p_erase;
    const double r = (double)rand() / (double)RAND_MAX;
    if (r < p_insert) {
        return POINT2_INSERT_OP;
    }
    if (r < p_insert + p_find) {
        return POINT2_FIND_OP;
    }
    return POINT2_ERASE_OP;
}

#endif

