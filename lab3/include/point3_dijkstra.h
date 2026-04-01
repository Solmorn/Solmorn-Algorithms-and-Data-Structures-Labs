#ifndef POINT3_DIJKSTRA_H
#define POINT3_DIJKSTRA_H

#include "point3_graph.h"

void dijkstra_naive(const Graph* graph, int source, long long* dist);
void dijkstra_binary(const Graph* graph, int source, long long* dist);
void dijkstra_binomial(const Graph* graph, int source, long long* dist);
void dijkstra_fibonacci(const Graph* graph, int source, long long* dist);

#endif