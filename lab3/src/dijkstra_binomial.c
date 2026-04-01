#include <limits.h>
#include <stdlib.h>

#include "../include/point3_dijkstra.h"
#include "../include/point2_binomial_heap.h"

static const long long INF = LLONG_MAX / 4;

void dijkstra_binomial(const Graph* graph, int source, long long* dist) {
    int* used = (int*)calloc((size_t)graph->n, sizeof(int));
    HeapElem** positions = (HeapElem**)calloc((size_t)graph->n, sizeof(HeapElem*));

    for (int i = 0; i < graph->n; ++i) {
        dist[i] = INF;
    }

    Heap* heap = heap_ctor_with_positions(positions);

    dist[source] = 0;
    heap_insert(heap, 0, source);

    while (!heap_empty(heap)) {
        HeapElem* min_node = heap_extract_min_node(heap);

        int v = min_node->request_index;
        long long cur_dist = min_node->value;

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
                    heap_insert(heap, dist[to], to);
                } else {
                    heap_decrease_key_to(heap, to, dist[to]);
                }
            }
        }
    }

    heap_dtor(heap);
    free(positions);
    free(used);
}