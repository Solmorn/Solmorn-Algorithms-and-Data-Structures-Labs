#include <limits.h>
#include <stdlib.h>

#include "../include/point3_dijkstra.h"
#include "../include/point1_heaps.h"

static const long long INF = LLONG_MAX / 4;

void dijkstra_binary(const Graph* graph, int source, long long* dist) {
    int* used = (int*)calloc((size_t)graph->n, sizeof(int));
    int* positions = (int*)calloc((size_t)graph->n, sizeof(int));

    for (int i = 0; i < graph->n; ++i) {
        dist[i] = INF;
        positions[i] = -1;
    }

    Heap* heap = heap_ctor((size_t)graph->n + 1, positions);

    dist[source] = 0;
    heap_insert(heap, 0, source);

    while (!heap_empty(heap)) {
        HeapElem min_elem = heap_extract_min_elem(heap);

        int v = min_elem.request_index;
        long long cur_dist = min_elem.value;

        if (used[v]) continue;
        used[v] = 1;

        if (cur_dist != dist[v]) continue;

        for (int edge_id = graph->head[v]; edge_id != -1; edge_id = graph->edges[edge_id].next) {
            int to = graph->edges[edge_id].to;
            int w = graph->edges[edge_id].weight;

            if (used[to]) continue;

            if (dist[v] + w < dist[to]) {
                dist[to] = dist[v] + w;

                if (positions[to] == -1) {
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