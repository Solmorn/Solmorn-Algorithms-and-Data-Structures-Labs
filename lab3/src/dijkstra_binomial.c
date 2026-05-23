#include <assert.h>
#include <stdlib.h>

#include "../include/point3_dijkstra.h"
#include "../include/point2_binomial_heap.h"

static HeapElem** dijkstra_binomial_positions_ctor(int n) {
    HeapElem** positions = (HeapElem**)calloc((size_t)n, sizeof(HeapElem*));
    assert(positions != NULL);

    return positions;
}

static void dijkstra_binomial_init_positions(HeapElem** positions, int n) {
    assert(positions != NULL);

    (void)n;
}

static Heap* dijkstra_binomial_heap_ctor(int n, HeapElem** positions) {
    assert(positions != NULL);

    (void)n;
    return heap_ctor_with_positions(positions);
}

#define DIJKSTRA_FUNC dijkstra_binomial
#define DIJKSTRA_POS_TYPE HeapElem**
#define DIJKSTRA_POSITIONS_CTOR dijkstra_binomial_positions_ctor
#define DIJKSTRA_INIT_POSITIONS dijkstra_binomial_init_positions
#define DIJKSTRA_POSITIONS_DTOR free
#define DIJKSTRA_HEAP_TYPE Heap
#define DIJKSTRA_HEAP_CTOR dijkstra_binomial_heap_ctor
#define DIJKSTRA_HEAP_DTOR heap_dtor
#define DIJKSTRA_HEAP_EMPTY heap_empty
#define DIJKSTRA_HEAP_INSERT heap_insert
#define DIJKSTRA_HEAP_DECREASE_KEY_TO heap_decrease_key_to
#define DIJKSTRA_HEAP_EXTRACT_MIN heap_extract_min_node
#define DIJKSTRA_MIN_TYPE HeapElem*
#define DIJKSTRA_MIN_VERTEX(min_node) ((min_node)->request_index)
#define DIJKSTRA_MIN_DIST(min_node) ((min_node)->value)
#define DIJKSTRA_MIN_DTOR(min_node) free(min_node)
#define DIJKSTRA_NEED_INSERT(positions, vertex) ((positions)[vertex] == NULL)

#include "../include/point3_dijkstra_impl.h"
