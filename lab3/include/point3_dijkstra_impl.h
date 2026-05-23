#ifndef POINT3_DIJKSTRA_IMPL_H
#define POINT3_DIJKSTRA_IMPL_H

#include <limits.h>
#include <stdlib.h>

static const long long INF = LLONG_MAX / 4;

void DIJKSTRA_FUNC(const Graph* graph, int source, long long* dist) {
    int* used = (int*)calloc((size_t)graph->n, sizeof(int));
    DIJKSTRA_POS_TYPE positions = DIJKSTRA_POSITIONS_CTOR(graph->n);

    for (int i = 0; i < graph->n; i++) {
        dist[i] = INF;
    }

    DIJKSTRA_INIT_POSITIONS(positions, graph->n);

    DIJKSTRA_HEAP_TYPE* heap = DIJKSTRA_HEAP_CTOR(graph->n, positions);

    dist[source] = 0;
    DIJKSTRA_HEAP_INSERT(heap, 0, source);

    while (!DIJKSTRA_HEAP_EMPTY(heap)) {
        DIJKSTRA_MIN_TYPE min_node = DIJKSTRA_HEAP_EXTRACT_MIN(heap);

        int v = DIJKSTRA_MIN_VERTEX(min_node);
        long long cur_dist = DIJKSTRA_MIN_DIST(min_node);

        DIJKSTRA_MIN_DTOR(min_node);

        if (used[v]) continue;
        used[v] = 1;

        if (cur_dist != dist[v]) continue;

        for (int edge_id = graph->head[v]; edge_id != -1; edge_id = graph->edges[edge_id].next) {
            int to = graph->edges[edge_id].to;
            int w = graph->edges[edge_id].weight;

            if (used[to]) continue;

            if (dist[v] + w < dist[to]) {
                dist[to] = dist[v] + w;

                if (DIJKSTRA_NEED_INSERT(positions, to)) {
                    DIJKSTRA_HEAP_INSERT(heap, dist[to], to);
                } else {
                    DIJKSTRA_HEAP_DECREASE_KEY_TO(heap, to, dist[to]);
                }
            }
        }
    }

    DIJKSTRA_HEAP_DTOR(heap);
    DIJKSTRA_POSITIONS_DTOR(positions);
    free(used);
}

#undef DIJKSTRA_FUNC
#undef DIJKSTRA_POS_TYPE
#undef DIJKSTRA_POSITIONS_CTOR
#undef DIJKSTRA_INIT_POSITIONS
#undef DIJKSTRA_POSITIONS_DTOR
#undef DIJKSTRA_HEAP_TYPE
#undef DIJKSTRA_HEAP_CTOR
#undef DIJKSTRA_HEAP_DTOR
#undef DIJKSTRA_HEAP_EMPTY
#undef DIJKSTRA_HEAP_INSERT
#undef DIJKSTRA_HEAP_DECREASE_KEY_TO
#undef DIJKSTRA_HEAP_EXTRACT_MIN
#undef DIJKSTRA_MIN_TYPE
#undef DIJKSTRA_MIN_VERTEX
#undef DIJKSTRA_MIN_DIST
#undef DIJKSTRA_MIN_DTOR
#undef DIJKSTRA_NEED_INSERT

#endif
