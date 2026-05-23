#include <stdlib.h>

#include "../include/point3_dijkstra.h"
#include "../include/point3_fibonacci_heap.h"

static FibNode** dijkstra_fibonacci_positions_ctor(int n) {
    return (FibNode**)calloc((size_t)n, sizeof(FibNode*));
}

static void dijkstra_fibonacci_init_positions(FibNode** positions, int n) {
    (void)positions;
    (void)n;
}

static FibHeap* dijkstra_fibonacci_heap_ctor(int n, FibNode** positions) {
    (void)n;
    return fib_heap_ctor(positions);
}

#define DIJKSTRA_FUNC dijkstra_fibonacci
#define DIJKSTRA_POS_TYPE FibNode**
#define DIJKSTRA_POSITIONS_CTOR dijkstra_fibonacci_positions_ctor
#define DIJKSTRA_INIT_POSITIONS dijkstra_fibonacci_init_positions
#define DIJKSTRA_POSITIONS_DTOR free
#define DIJKSTRA_HEAP_TYPE FibHeap
#define DIJKSTRA_HEAP_CTOR dijkstra_fibonacci_heap_ctor
#define DIJKSTRA_HEAP_DTOR fib_heap_dtor
#define DIJKSTRA_HEAP_EMPTY fib_heap_empty
#define DIJKSTRA_HEAP_INSERT fib_heap_insert
#define DIJKSTRA_HEAP_DECREASE_KEY_TO fib_heap_decrease_key_to
#define DIJKSTRA_HEAP_EXTRACT_MIN fib_heap_extract_min_node
#define DIJKSTRA_MIN_TYPE FibNode*
#define DIJKSTRA_MIN_VERTEX(min_node) ((min_node)->vertex)
#define DIJKSTRA_MIN_DIST(min_node) ((min_node)->key)
#define DIJKSTRA_MIN_DTOR(min_node) free(min_node)
#define DIJKSTRA_NEED_INSERT(positions, vertex) ((positions)[vertex] == NULL)

#include "../include/point3_dijkstra_impl.h"
