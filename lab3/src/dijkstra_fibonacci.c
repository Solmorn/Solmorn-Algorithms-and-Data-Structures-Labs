#include <limits.h>
#include <stdlib.h>

#include "../include/point3_dijkstra.h"
#include "../include/point3_fibonacci_heap.h"

static const long long INF = LLONG_MAX / 4;

void dijkstra_fibonacci(const Graph* graph, int source, long long* dist) {
    int* used = (int*)calloc((size_t)graph->n, sizeof(int));
    FibNode** positions = (FibNode**)calloc((size_t)graph->n, sizeof(FibNode*));

    for (int i = 0; i < graph->n; ++i) {
        dist[i] = INF;
    }

    FibHeap* heap = fib_heap_ctor(positions);

    dist[source] = 0;
    fib_heap_insert(heap, 0, source);

    while (!fib_heap_empty(heap)) {
        FibNode* min_node = fib_heap_extract_min_node(heap);

        int v = min_node->vertex;
        long long cur_dist = min_node->key;

        free(min_node);

        if (used[v]) continue;
        used[v] = 1;

        if (cur_dist != dist[v]) continue;

        for (int edge_id = graph->head[v]; edge_id != -1; edge_id = graph->edges[edge_id].next) {
            int to = graph->edges[edge_id].to;
            int w = graph->edges[edge_id].weight;

            if (used[to]) continue;

            if (dist[v] + w < dist[to]) {
                dist[to] = dist[v] + w;

                if (positions[to] == NULL) {
                    fib_heap_insert(heap, dist[to], to);
                } else {
                    fib_heap_decrease_key_to(heap, to, dist[to]);
                }
            }
        }
    }

    fib_heap_dtor(heap);
    free(positions);
    free(used);
}