#include <limits.h>
#include <stdlib.h>

#include "../include/point3_dijkstra.h"

static const long long INF = LLONG_MAX / 4;

void dijkstra_naive(const Graph* graph, int source, long long* dist) {
    int* used = (int*)calloc((size_t)graph->n, sizeof(int));

    for (int i = 0; i < graph->n; ++i) {
        dist[i] = INF;
    }

    dist[source] = 0;

    for (int iter = 0; iter < graph->n; ++iter) {
        int v = -1;

        for (int i = 0; i < graph->n; ++i) {
            if (used[i]) continue;

            if (v == -1 || dist[i] < dist[v]) {
                v = i;
            }
        }

        if (v == -1 || dist[v] == INF) {
            break;
        }

        used[v] = 1;

        for (int edge_id = graph->head[v]; edge_id != -1; edge_id = graph->edges[edge_id].next) {
            int to = graph->edges[edge_id].to;
            int w = graph->edges[edge_id].weight;

            if (dist[v] + w < dist[to]) {
                dist[to] = dist[v] + w;
            }
        }
    }

    free(used);
}
