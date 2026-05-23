#include <stdlib.h>

#include "../include/point3_dijkstra.h"
#include "../include/point1_heaps.h"

static int* dijkstra_binary_positions_ctor(int n) {
    return (int*)calloc((size_t)n, sizeof(int));
}

static void dijkstra_binary_init_positions(int* positions, int n) {
    for (int i = 0; i < n; i++) {
        positions[i] = -1;
    }
}

static Heap* dijkstra_binary_heap_ctor(int n, int* positions) {
    return heap_ctor((size_t)n + 1, positions);
}

#define DIJKSTRA_FUNC dijkstra_binary
#define DIJKSTRA_POS_TYPE int*
#define DIJKSTRA_POSITIONS_CTOR dijkstra_binary_positions_ctor
#define DIJKSTRA_INIT_POSITIONS dijkstra_binary_init_positions
#define DIJKSTRA_POSITIONS_DTOR free
#define DIJKSTRA_HEAP_TYPE Heap
#define DIJKSTRA_HEAP_CTOR dijkstra_binary_heap_ctor
#define DIJKSTRA_HEAP_DTOR heap_dtor
#define DIJKSTRA_HEAP_EMPTY heap_empty
#define DIJKSTRA_HEAP_INSERT heap_insert
#define DIJKSTRA_HEAP_DECREASE_KEY_TO heap_decrease_key_to
#define DIJKSTRA_HEAP_EXTRACT_MIN heap_extract_min_elem
#define DIJKSTRA_MIN_TYPE HeapElem
#define DIJKSTRA_MIN_VERTEX(min_node) ((min_node).request_index)
#define DIJKSTRA_MIN_DIST(min_node) ((min_node).value)
#define DIJKSTRA_MIN_DTOR(min_node) ((void)(min_node))
#define DIJKSTRA_NEED_INSERT(positions, vertex) ((positions)[vertex] == -1)

#include "../include/point3_dijkstra_impl.h"
