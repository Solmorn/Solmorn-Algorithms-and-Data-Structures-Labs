#ifndef POINT3_GRAPH_H
#define POINT3_GRAPH_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int to;
    int weight;
    int next;
} Edge;

typedef struct {
    int n;
    int m;

    int* head;
    Edge* edges;
    int edge_count;
} Graph;

static Graph* graph_ctor(int n, int m) {
    Graph* graph = (Graph*)calloc(1, sizeof(Graph));
    assert(graph != NULL);

    graph->n = n;
    graph->m = m;
    graph->head = (int*)calloc((size_t)n, sizeof(int));
    graph->edges = (Edge*)calloc((size_t)(2 * m), sizeof(Edge));
    graph->edge_count = 0;

    assert(graph->head != NULL);
    assert(graph->edges != NULL);

    for (int i = 0; i < n; ++i) {
        graph->head[i] = -1;
    }

    return graph;
}

static void graph_add_directed_edge(Graph* graph, int from, int to, int weight) {
    assert(graph);
    assert(0 <= from && from < graph->n);
    assert(0 <= to && to < graph->n);

    int id = graph->edge_count++;

    graph->edges[id].to = to;
    graph->edges[id].weight = weight;
    graph->edges[id].next = graph->head[from];
    graph->head[from] = id;
}

static void graph_add_undirected_edge(Graph* graph, int u, int v, int weight) {
    graph_add_directed_edge(graph, u, v, weight);
    graph_add_directed_edge(graph, v, u, weight);
}

static Graph* graph_read_binary(const char* path) {
    FILE* file = fopen(path, "rb");
    assert(file != NULL);

    int n = 0;
    int m = 0;

    assert(fread(&n, sizeof(int), 1, file) == 1);
    assert(fread(&m, sizeof(int), 1, file) == 1);

    Graph* graph = graph_ctor(n, m);

    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        int w = 0;

        assert(fread(&u, sizeof(int), 1, file) == 1);
        assert(fread(&v, sizeof(int), 1, file) == 1);
        assert(fread(&w, sizeof(int), 1, file) == 1);

        graph_add_undirected_edge(graph, u, v, w);
    }

    fclose(file);
    return graph;
}

static void graph_dtor(Graph* graph) {
    if (graph == NULL) return;

    free(graph->head);
    free(graph->edges);
    free(graph);
}

#endif
